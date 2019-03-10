#include <discoverymanager.h>
#include <crossplatform.h>
#include <applicationcore.h>

#include <QUdpSocket>
#include <QWebSocket>
#include <QTimerEvent>
#include <QDataStream>

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

QUrl hostAddressToUrl(const QHostAddress& address, int port)
{
    if (address.protocol() == QAbstractSocket::IPv6Protocol)
        return QString("ws://[%1]:%2").arg(address.toString()).arg(port);
    else
        return QString("ws://%1:%2").arg(address.toString()).arg(port);
}

void dispatch(const QByteArray& incomingData, QByteArray& data, QString& command)
{
    QDataStream incoming(incomingData);
    incoming.setVersion(QDataStream::Qt_5_12);
    incoming >> command;
    incoming >> data;
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
bool DiscoveryManager::s_connected = false;
QString DiscoveryManager::s_address;

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
        s_webSocket->sendBinaryMessage(serialize(pushValues(ApplicationCore::deviceInfo()), "DeviceInfo"));
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
    QWebSocket* client = static_cast<QWebSocket*>(sender());
    QByteArray data;
    QString command;
    dispatch(incomingData, data, command);

    if (command == "DeviceInfo") {
//        QVariantMap info;
//        pullValues(data, info);
//        s_deviceInfoList.append(info);
//        client->setProperty(UID_PROPERTY, info.value("deviceUid").toString());
//        emit connected(info);
    } else {

    }
}