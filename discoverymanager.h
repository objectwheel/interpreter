#ifndef DISCOVERYMANAGER_H
#define DISCOVERYMANAGER_H

#include <QObject>
#include <QBasicTimer>
#include <QDataStream>
#include <QWebSocket>
#include <utilityfunctions.h>

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
    enum DiscoveryCommands {
        Broadcast = 0x1100,
        Execute,
        Terminate,
        InfoReport,
        StartReport,
        OutputReport,
        ExitReport
    };

public:
    static DiscoveryManager* instance();

    static bool isConnected();

    static QString address();

    static void setDisabled(bool disabled);

    template<typename... Args>
    static void send(DiscoveryCommands command, Args&&... args)
    {
        using namespace UtilityFunctions;
        if (!isConnected()) {
            qWarning("WARNING: Cannot send any data since there is no active connection");
            return;
        }
        s_webSocket->sendBinaryMessage(push(command, push(std::forward<Args>(args)...)));
    }

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
    void terminate();

private:
    static DiscoveryManager* s_instance;
    static QBasicTimer s_emulatorTimer;
    static QUdpSocket* s_broadcastSocket;
    static QWebSocket* s_webSocket;
    static QString s_address;
    static bool s_connected;
};

Q_DECLARE_METATYPE(DiscoveryManager::DiscoveryCommands)

inline QDataStream& operator>>(QDataStream& in, DiscoveryManager::DiscoveryCommands& e)
{ return in >> (int&) e; }

inline QDataStream& operator<<(QDataStream& out, const DiscoveryManager::DiscoveryCommands& e)
{ return out << int(e); }

#endif // DISCOVERYMANAGER_H