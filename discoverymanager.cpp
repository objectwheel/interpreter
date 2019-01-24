#include <discoverymanager.h>
#include <QUdpSocket>
#include <QWebSocket>
#include <QTimerEvent>

#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#endif

namespace {

bool isAndroidEmulator()
{
#if defined(Q_OS_ANDROID)
    return QAndroidJniObject::callStaticMethod<jboolean>(
                "com/objectwheel/testemulator/TestEmulator",
                "isEmulator");
#else
    return false;
#endif
}

QUrl hostAddressToUrl(const QHostAddress& address, int port)
{
    if (address.protocol() == QAbstractSocket::IPv6Protocol)
        return QString("ws://[%1]:%2").arg(address.toString()).arg(port);
    else
        return QString("ws://%1:%2").arg(address.toString()).arg(port);
}
}

const QByteArray DiscoveryManager::BROADCAST_MESSAGE = "Objectwheel Device Discovery Broadcast";

QUdpSocket* DiscoveryManager::s_broadcastSocket = nullptr;
QWebSocket* DiscoveryManager::s_webSocket = nullptr;

DiscoveryManager::DiscoveryManager(QObject* parent) : QObject(parent)
{
    s_broadcastSocket = new QUdpSocket(this);
    s_webSocket = new QWebSocket(QStringLiteral(), QWebSocketProtocol::VersionLatest, this);
    s_broadcastSocket->bind(BROADCAST_PORT, QUdpSocket::ShareAddress);

    connect(s_broadcastSocket, &QUdpSocket::readyRead,
            this, &DiscoveryManager::onBroadcastReadyRead);

    connect(s_broadcastSocket, QOverload<QAbstractSocket::SocketError>::of(&QUdpSocket::error),
            this, [=] (QAbstractSocket::SocketError socketError) {
        qWarning() << "DiscoveryManager: Broadcast socket error" << socketError;
    });

    if (isAndroidEmulator()) {
//        QTimer::singleShot(1000, this, [=] {
//            qDebug("@@@@@@@@@@@@@@@@@");
//        });
    }

    connect(s_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, [=] {
        qDebug() << "error";
    });
    connect(s_webSocket, &QWebSocket::sslErrors, this, [=] {
        qDebug() << "serverErrors";
    });
    connect(s_webSocket, &QWebSocket::aboutToClose, this, [=] {
        qDebug() << "aboutToClose";
    });
    connect(s_webSocket, &QWebSocket::connected, this, [=] {
        qDebug() << "connected";
    });
    connect(s_webSocket, &QWebSocket::disconnected, this, [=] {
        qDebug() << "disconnected";
    });
    connect(s_webSocket, &QWebSocket::sslErrors, this, [=] {
        qDebug() << "sslErrors";
    });

    //    void binaryFrameReceived(const QByteArray &frame, bool isLastFrame)
    //    void binaryMessageReceived(const QByteArray &message)
    //    void textFrameReceived(const QString &frame, bool isLastFrame)
    //    void textMessageReceived(const QString &message)

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
        qDebug() << hostAddressToUrl(address, SERVER_PORT);
        s_broadcastSocket->close();
        s_webSocket->open(hostAddressToUrl(address, SERVER_PORT));
    }
}