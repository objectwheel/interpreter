#include <discoverymanager.h>
#include <hashfactory.h>

#include <QUdpSocket>
#include <QWebSocket>
#include <QTimerEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>

#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#endif

namespace {

void pushValuesHelper(QDataStream&) {}

template <typename Arg, typename... Args>
void pushValuesHelper(QDataStream& stream, const Arg& arg, const Args&... args) {
    stream << arg;
    pushValuesHelper(stream, args...);
}

template <typename... Args>
QByteArray pushValues(const Args&... args) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_12);
    pushValuesHelper(stream, args...);
    return data;
}

void pullValuesHelper(QDataStream&) {}

template <typename Arg, typename... Args>
void pullValuesHelper(QDataStream& stream, Arg& arg, Args&... args) {
    stream >> arg;
    pullValuesHelper(stream, args...);
}

template <typename... Args>
void pullValues(const QByteArray& data, Args&... args) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_12);
    pullValuesHelper(stream, args...);
}

bool isAndroidEmulator()
{
#if defined(Q_OS_ANDROID)
    static bool emulator = QAndroidJniObject::callStaticMethod<jboolean>(
                "com/objectwheel/testemulator/TestEmulator", "isEmulator");
#else
    static bool emulator = false;
#endif
    return emulator;
}

QUrl hostAddressToUrl(const QHostAddress& address, int port)
{
    if (address.protocol() == QAbstractSocket::IPv6Protocol)
        return QString("ws://[%1]:%2").arg(address.toString()).arg(port);
    else
        return QString("ws://%1:%2").arg(address.toString()).arg(port);
}

QVariantMap deviceInfo()
{
    static const QJsonObject info = {
        {"deviceUid", HashFactory::generate()},
        {"buildCpuArchitecture", QSysInfo::buildCpuArchitecture()},
        {"currentCpuArchitecture", QSysInfo::currentCpuArchitecture()},
        {"buildAbi", QSysInfo::buildAbi()},
        {"kernelType", QSysInfo::kernelType()},
        {"kernelVersion", QSysInfo::kernelVersion()},
        {"productType", QSysInfo::productType()},
        {"productVersion", QSysInfo::productVersion()},
        {"prettyProductName", QSysInfo::prettyProductName()},
        {"machineHostName", QSysInfo::machineHostName()}
    };
    return info.toVariantMap();
}

QByteArray serialize(const QByteArray& data, const QString& command)
{
    QByteArray outgoingData;
    QDataStream outgoing(&outgoingData, QIODevice::WriteOnly);
    outgoing.setVersion(QDataStream::Qt_5_12);
    outgoing << command;
    outgoing << data;
    return outgoingData;
}
}

const QByteArray DiscoveryManager::BROADCAST_MESSAGE = "Objectwheel Device Discovery Broadcast";
DiscoveryManager* DiscoveryManager::s_instance = nullptr;
QBasicTimer DiscoveryManager::s_emulatorTimer;
QUdpSocket* DiscoveryManager::s_broadcastSocket = nullptr;
QWebSocket* DiscoveryManager::s_webSocket = nullptr;

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
    connect(s_webSocket, &QWebSocket::disconnected, this, &DiscoveryManager::disconnected);
    connect(s_webSocket, &QWebSocket::connected, this, [=] {
        if (isAndroidEmulator())
            stop();
        s_webSocket->sendBinaryMessage(serialize(pushValues(deviceInfo()), "DeviceInfo"));
        emit connected();
    });

    //    void binaryFrameReceived(const QByteArray &frame, bool isLastFrame)
    //    void binaryMessageReceived(const QByteArray &message)
    //    void textFrameReceived(const QString &frame, bool isLastFrame)
    //    void textMessageReceived(const QString &message)

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

void DiscoveryManager::start()
{
    if (isAndroidEmulator())
        s_emulatorTimer.start(1000, this);
    else
        s_broadcastSocket->bind(BROADCAST_PORT, QUdpSocket::ShareAddress);
}

void DiscoveryManager::stop()
{
    if (isAndroidEmulator())
        s_emulatorTimer.stop();
    else
        s_broadcastSocket->close();
}

void DiscoveryManager::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == s_emulatorTimer.timerId())
        s_webSocket->open(hostAddressToUrl(QHostAddress("10.0.2.2"), SERVER_PORT));
    else
        QObject::timerEvent(event);
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
        s_webSocket->open(hostAddressToUrl(address, SERVER_PORT));
    }
}