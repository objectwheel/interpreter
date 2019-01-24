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
}

const QByteArray DiscoveryManager::SERVER_NAME = "Objectwheel Device Manager";
const QByteArray DiscoveryManager::BROADCAST_MESSAGE = "Objectwheel Device Discovery Broadcast";

QBasicTimer DiscoveryManager::s_broadcastTimer;
QUdpSocket* DiscoveryManager::s_broadcastSocket = nullptr;
QWebSocketServer* DiscoveryManager::s_webSocket = nullptr;

DiscoveryManager::DiscoveryManager(QObject* parent) : QObject(parent)
{
    s_broadcastSocket = new QUdpSocket(this);
    s_webSocket = new QWebSocketServer(SERVER_NAME, QWebSocketServer::SecureMode, this);
    s_webSocket->listen(QHostAddress::Any, SERVER_PORT);

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(15963, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));

    if (isAndroidEmulator()) {
        QTimer::singleShot(1000, this, [=] {
            qDebug("@@@@@@@@@@@@@@@@@");
        });
    }

//    void aboutToClose()
//    void binaryFrameReceived(const QByteArray &frame, bool isLastFrame)
//    void binaryMessageReceived(const QByteArray &message)
//    void bytesWritten(qint64 bytes)
//    void connected()
//    void disconnected()
//    void error(QAbstractSocket::SocketError error)
//    void pong(quint64 elapsedTime, const QByteArray &payload)
//    void preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
//    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
//    void readChannelFinished()
//    void sslErrors(const QList<QSslError> &errors)
//    void stateChanged(QAbstractSocket::SocketState state)
//    void textFrameReceived(const QString &frame, bool isLastFrame)
//    void textMessageReceived(const QString &message)

}

void DiscoveryManager::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == s_broadcastTimer.timerId())
        s_broadcastSocket->writeDatagram(BROADCAST_MESSAGE, QHostAddress::Broadcast, BROADCAST_PORT);
    else
        QObject::timerEvent(event);
}

void Receiver::processPendingDatagrams()
{
    QByteArray datagram;
//! [2]
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        statusLabel->setText(tr("Received datagram: \"%1\"")
                             .arg(datagram.constData()));
    }
//! [2]
}