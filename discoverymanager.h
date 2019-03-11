#ifndef DISCOVERYMANAGER_H
#define DISCOVERYMANAGER_H

#include <QObject>
#include <QBasicTimer>

class QUdpSocket;
class QWebSocket;
class QHostAddress;

class DiscoveryManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DiscoveryManager)

    friend class ApplicationCore;

    enum {
        BROADCAST_PORT = 15425,
        SERVER_PORT = 15426,
    };

public:
    static DiscoveryManager* instance();

    static bool isConnected();

    static QString address();

    static void setDisabled(bool disabled);

private:
    explicit DiscoveryManager(QObject* parent = nullptr);
    ~DiscoveryManager() override;

private slots:
    void start();
    void stop();
    void onBroadcastReadyRead();
    void onBinaryMessageReceived(const QByteArray& incomingData);

private:
    QUrl hostAddressToUrl(const QHostAddress& address, int port);

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
    static QString s_address;
    static bool s_connected;
};

#endif // DISCOVERYMANAGER_H