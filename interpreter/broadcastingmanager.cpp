#include <broadcastingmanager.h>
#include <crossplatform.h>
#include <applicationcore.h>

#include <QUdpSocket>
#include <QTimerEvent>
#include <QTemporaryFile>

using namespace UtilityFunctions;

BroadcastingManager* BroadcastingManager::s_instance = nullptr;
QBasicTimer BroadcastingManager::s_emulatorTimer;
QUdpSocket* BroadcastingManager::s_broadcastSocket = nullptr;
QWebSocket* BroadcastingManager::s_webSocket = nullptr;
QTemporaryFile* BroadcastingManager::s_cacheFile = nullptr;
QString BroadcastingManager::s_address;
bool BroadcastingManager::s_connected = false;

BroadcastingManager::BroadcastingManager(QObject* parent) : QObject(parent)
{
    s_instance = this;
    s_broadcastSocket = new QUdpSocket(this);
    s_webSocket = new QWebSocket(QStringLiteral(), QWebSocketProtocol::VersionLatest, this);

    connect(s_broadcastSocket, &QUdpSocket::readyRead,
            this, &BroadcastingManager::onBroadcastReadyRead);
    connect(s_broadcastSocket, QOverload<QAbstractSocket::SocketError>::of(&QUdpSocket::error),
            this, [=] (QAbstractSocket::SocketError socketError) {
        qWarning() << "BroadcastingManager: Broadcast socket error" << socketError;
    });
    connect(s_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, [=] (QAbstractSocket::SocketError socketError) {
        qWarning() << "BroadcastingManager: Server error" << socketError;
    });
    connect(s_webSocket, &QWebSocket::disconnected, this, &BroadcastingManager::start);
    connect(s_webSocket, &QWebSocket::disconnected, this, [=] {
        cleanExecutionCache();
        s_connected = false;
        emit disconnected();
    });
    connect(s_webSocket, &QWebSocket::connected, this, [=] {
        if (CrossPlatform::isAndroidEmulator())
            stop();
        s_connected = true;
        send(InfoReport, ApplicationCore::deviceInfo());
        emit connected();
    });
    connect(s_webSocket, &QWebSocket::binaryMessageReceived,
            this, &BroadcastingManager::onBinaryMessageReceived);

    start();
}

BroadcastingManager::~BroadcastingManager()
{
    s_instance = nullptr;
}

QUrl BroadcastingManager::hostAddressToUrl(const QHostAddress& address, int port)
{
    if (address.protocol() == QAbstractSocket::IPv6Protocol)
        return QString("ws://[%1]:%2").arg(address.toString()).arg(port);
    else
        return QString("ws://%1:%2").arg(address.toString()).arg(port);
}

template<typename... Args>
void BroadcastingManager::send(BroadcastingManager::Commands command, Args&&... args)
{
    using namespace UtilityFunctions;
    if (!isConnected()) {
        qWarning("WARNING: Cannot send any data since there is no active connection");
        return;
    }
    s_webSocket->sendBinaryMessage(push(command, push(std::forward<Args>(args)...)));
}

BroadcastingManager* BroadcastingManager::instance()
{
    return s_instance;
}

bool BroadcastingManager::isConnected()
{
    return s_connected;
}

QString BroadcastingManager::address()
{
    return s_address;
}

void BroadcastingManager::setDisabled(bool disabled)
{
    if (!instance())
        return;

    if (disabled) {
        if (s_webSocket->state() != QAbstractSocket::UnconnectedState)
            s_webSocket->abort();
        instance()->stop();
    } else {
        instance()->start();
    }
}

void BroadcastingManager::cleanExecutionCache()
{
    if (s_cacheFile)
        delete s_cacheFile;
    s_cacheFile = nullptr;
}

void BroadcastingManager::sendStartReport(const QString& projectDirectory)
{
    send(BroadcastingManager::StartReport, projectDirectory);
}

void BroadcastingManager::sendFinishReport(int exitCode, bool crashExit)
{
    send(BroadcastingManager::FinishReport, exitCode, crashExit);
}

void BroadcastingManager::sendProgressReport(int progress)
{
    send(BroadcastingManager::ProgressReport, progress);
}

void BroadcastingManager::sendOutputReport(const QString& output)
{
    send(BroadcastingManager::OutputReport, output);
}

void BroadcastingManager::sendErrorReport(const QString& errorString)
{
    send(BroadcastingManager::ErrorReport, errorString);
}

void BroadcastingManager::start()
{
    if (CrossPlatform::isAndroidEmulator())
        s_emulatorTimer.start(1000, this);
    else
        s_broadcastSocket->bind(BROADCAST_PORT, QUdpSocket::ShareAddress);
}

void BroadcastingManager::stop()
{
    if (CrossPlatform::isAndroidEmulator())
        s_emulatorTimer.stop();
    else
        s_broadcastSocket->abort();
}

void BroadcastingManager::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == s_emulatorTimer.timerId()) {
        s_address = "10.0.2.2";
        s_webSocket->open(hostAddressToUrl(QHostAddress(s_address), SERVER_PORT));
    } else {
        QObject::timerEvent(event);
    }
}

void BroadcastingManager::onBroadcastReadyRead()
{
    QByteArray datagram;
    QHostAddress address;
    Commands command;
    while (s_broadcastSocket->hasPendingDatagrams()) {
        datagram.resize(int(s_broadcastSocket->pendingDatagramSize()));
        s_broadcastSocket->readDatagram(datagram.data(), datagram.size(), &address);
    }
    pull(datagram, command);
    if (command == Broadcast) {
        stop();
        s_address = address.toString();
        s_webSocket->open(hostAddressToUrl(QHostAddress(s_address), SERVER_PORT));
    }
}

void BroadcastingManager::onBinaryMessageReceived(const QByteArray& incomingData)
{
    QByteArray data;
    Commands command;
    pull(incomingData, command, data);

    switch (command) {
    case Execute: {
        QString projectUid;
        int progress;
        qint64 pos;
        QByteArray chunkData;
        pull(data, projectUid, progress, pos, chunkData);

        if (pos == 0) {
            s_cacheFile = new QTemporaryFile(this);
            if (!s_cacheFile->open()) {
                qFatal("CRITICAL: Cannot create a temporary file");
                break;
            }
        }

        if (!s_cacheFile) {
            qWarning("WARNING: Cache corruption");
            break;
        }

        s_cacheFile->seek(pos);
        s_cacheFile->write(chunkData);

        emit downloadProgress(50 + progress / 2);

        if (!projectUid.isEmpty()) { // EOF
            s_cacheFile->close();
            emit execute(projectUid, s_cacheFile->fileName());
        }

        break;
    }

    case ProgressReport: {
        int progress;
        UtilityFunctions::pull(data, progress);
        emit downloadProgress(progress / 2);
        break;
    }

    case UploadStarted: {
        cleanExecutionCache();
        emit internalTermination();
        emit downloadStarted();
        break;
    }

    case Terminate: {
        cleanExecutionCache();
        emit terminate();
        break;
    }

    default:
        cleanExecutionCache();
        qWarning("BroadcastingManager: Unrecognized command has arrived");
        break;
    }
}
