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
class QTemporaryFile;

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
        FinishReport
    };

public:
    static DiscoveryManager* instance();
    static bool isConnected();
    static QString address();
    static void setDisabled(bool disabled);

public slots:
    static void scheduleStartReport();
    static void scheduleFinishReport(int exitCode);
    static void scheduleOutputReport(const QString& output);

private slots:
    void start();
    void stop();
    void onBroadcastReadyRead();
    void onBinaryMessageReceived(const QByteArray& incomingData);

private:
    void cleanCache();
    QUrl hostAddressToUrl(const QHostAddress& address, int port);

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

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    explicit DiscoveryManager(QObject* parent = nullptr);
    ~DiscoveryManager() override;

signals:
    void connected();
    void disconnected();
    void terminate();
    void execute(const QString& uid, const QString& projectPath);

private:
    static DiscoveryManager* s_instance;
    static QBasicTimer s_emulatorTimer;
    static QUdpSocket* s_broadcastSocket;
    static QWebSocket* s_webSocket;
    static QTemporaryFile* s_cacheFile;
    static QString s_address;
    static bool s_connected;
};

Q_DECLARE_METATYPE(DiscoveryManager::DiscoveryCommands)

inline QDataStream& operator>>(QDataStream& in, DiscoveryManager::DiscoveryCommands& e)
{ return in >> (int&) e; }

inline QDataStream& operator<<(QDataStream& out, const DiscoveryManager::DiscoveryCommands& e)
{ return out << int(e); }

#endif // DISCOVERYMANAGER_H