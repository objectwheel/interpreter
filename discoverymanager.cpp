#include <discoverymanager.h>
#include <crossplatform.h>
#include <applicationcore.h>

#include <QUdpSocket>
#include <QWebSocket>
#include <QTimerEvent>
#include <QDataStream>

DiscoveryManager* DiscoveryManager::s_instance = nullptr;
QBasicTimer DiscoveryManager::s_emulatorTimer;
QUdpSocket* DiscoveryManager::s_broadcastSocket = nullptr;
QWebSocket* DiscoveryManager::s_webSocket = nullptr;
QString DiscoveryManager::s_address;
bool DiscoveryManager::s_connected = false;

DiscoveryManager::DiscoveryManager(QObject* parent) : QObject(parent)
{
    s_instance = this;
    s_broadcastSocket = new QUdpSocket(this);
    s_webSocket = new QWebSocket(QStringLiteral(), QWebSocketProtocol::VersionLatest, this);

    connect(s_broadcastSocket, &QUdpSocket::readyRead,
            this, &DiscoveryManager::onBroadcastReadyRead);
    connect(s_broadcastSocket, QOverload<QAbstractSocket::SocketError>::of(&QUdpSocket::error),
            this, [=] (QAbstractSocket::SocketError socketError) {
        qWarning() << "DiscoveryManager: Broadcast socket error" << socketError;
    });
    connect(s_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, [=] (QAbstractSocket::SocketError socketError) {
        qWarning() << "DiscoveryManager: Server error" << socketError;
    });
    connect(s_webSocket, &QWebSocket::disconnected, this, &DiscoveryManager::start);
    connect(s_webSocket, &QWebSocket::disconnected, this, [=] {
        s_connected = false;
        emit disconnected();
    });
    connect(s_webSocket, &QWebSocket::connected, this, [=] {
        if (CrossPlatform::isAndroidEmulator())
            stop();
        s_webSocket->sendBinaryMessage(serialize(push(ApplicationCore::deviceInfo()), "DeviceInfo"));
        s_connected = true;
        emit connected();
    });
    connect(s_webSocket, &QWebSocket::binaryMessageReceived,
            this, &DiscoveryManager::onBinaryMessageReceived);

    start();
}

DiscoveryManager::~DiscoveryManager()
{
    s_instance = nullptr;
}

DiscoveryManager* DiscoveryManager::instance()
{
    return s_instance;
}

bool DiscoveryManager::isConnected()
{
    return s_connected;
}

QString DiscoveryManager::address()
{
    return s_address;
}

void DiscoveryManager::setDisabled(bool disabled)
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

void DiscoveryManager::start()
{
    if (CrossPlatform::isAndroidEmulator())
        s_emulatorTimer.start(1000, this);
    else
        s_broadcastSocket->bind(BROADCAST_PORT, QUdpSocket::ShareAddress);
}

void DiscoveryManager::stop()
{
    if (CrossPlatform::isAndroidEmulator())
        s_emulatorTimer.stop();
    else
        s_broadcastSocket->abort();
}

QUrl DiscoveryManager::hostAddressToUrl(const QHostAddress& address, int port)
{
    if (address.protocol() == QAbstractSocket::IPv6Protocol)
        return QString("ws://[%1]:%2").arg(address.toString()).arg(port);
    else
        return QString("ws://%1:%2").arg(address.toString()).arg(port);
}

void DiscoveryManager::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == s_emulatorTimer.timerId()) {
        s_address = "10.0.2.2";
        s_webSocket->open(hostAddressToUrl(QHostAddress(s_address), SERVER_PORT));
    } else {
        QObject::timerEvent(event);
    }
}

void DiscoveryManager::onBroadcastReadyRead()
{
    QByteArray datagram;
    QHostAddress address;
    while (s_broadcastSocket->hasPendingDatagrams()) {
        datagram.resize(int(s_broadcastSocket->pendingDatagramSize()));
        s_broadcastSocket->readDatagram(datagram.data(), datagram.size(), &address);
    }
    if (datagram == BROADCAST_MESSAGE) {
        stop();
        s_address = address.toString();
        s_webSocket->open(hostAddressToUrl(QHostAddress(s_address), SERVER_PORT));
    }
}

void DiscoveryManager::onBinaryMessageReceived(const QByteArray& incomingData)
{
    QByteArray data;
    QString command;
    dispatch(incomingData, data, command);

    if (command == "DeviceInfo") {
//        QVariantMap info;
//        pull(data, info);
//        s_deviceInfoList.append(info);
//        client->setProperty(UID_PROPERTY, info.value("deviceUid").toString());
//        emit connected(info);
    } else {

    }
}
