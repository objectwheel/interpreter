#ifndef DISCOVERYMANAGER_H
#define DISCOVERYMANAGER_H

#include <QObject>
#include <QBasicTimer>

class QUdpSocket;
class QWebSocketServer;

class DiscoveryManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DiscoveryManager)

    friend class ApplicationCore;

    enum {
        BROADCAST_PORT = 15425,
        SERVER_PORT = 15426,
    };
    static const QByteArray SERVER_NAME;
    static const QByteArray BROADCAST_MESSAGE;

private:
    explicit DiscoveryManager(QObject* parent = nullptr);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    static QBasicTimer s_broadcastTimer;
    static QUdpSocket* s_broadcastSocket;
    static QWebSocketServer* s_webSocket;
};
#endif // DISCOVERYMANAGER_H