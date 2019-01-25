#ifndef DISCOVERYMANAGER_H
#define DISCOVERYMANAGER_H

#include <QObject>
#include <QBasicTimer>

class QUdpSocket;
class QWebSocket;

class DiscoveryManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DiscoveryManager)

    friend class ApplicationCore;

    enum {
        BROADCAST_PORT = 15425,
        SERVER_PORT = 15426,
    };
    static const QByteArray BROADCAST_MESSAGE;

private:
    explicit DiscoveryManager(QObject* parent = nullptr);
    ~DiscoveryManager() override;

    static DiscoveryManager* instance();

private slots:
    void start();
    void stop();
    void onBroadcastReadyRead();

protected:
    void timerEvent(QTimerEvent* event) override;

signals:
    void connected();
    void disconnected();

private:
    static DiscoveryManager* s_instance;
    static QBasicTimer s_emulatorTimer;
    static QUdpSocket* s_broadcastSocket;
    static QWebSocket* s_webSocket;
};

#endif // DISCOVERYMANAGER_H