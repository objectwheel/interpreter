#ifndef BROADCASTINGMANAGER_H
#define BROADCASTINGMANAGER_H

#include <QObject>
#include <QBasicTimer>
#include <QDataStream>
#include <QWebSocket>
#include <utilityfunctions.h>

class QUdpSocket;
class QWebSocket;
class QHostAddress;
class QTemporaryFile;

class BroadcastingManager final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(BroadcastingManager)

    friend class ApplicationCore;

    enum {
        BROADCAST_PORT = 15425,
        SERVER_PORT = 15426,
    };

public:
    enum Commands : quint32 {
        Broadcast = 0x1100,
        Execute,
        Terminate,
        InfoReport,
        StartReport,
        OutputReport,
        FinishReport,
        ProgressReport,
        UploadStarted,
        ErrorReport
    };

public:
    static BroadcastingManager* instance();
    static bool isConnected();
    static QString address();
    static void setDisabled(bool disabled);

public slots:
    static void cleanExecutionCache();
    static void sendStartReport(const QString& projectDirectory);
    static void sendFinishReport(int exitCode, bool crashExit);
    static void sendProgressReport(int progress);
    static void sendOutputReport(const QString& output);
    static void sendErrorReport(const QString& errorString);

private slots:
    void start();
    void stop();
    void onBroadcastReadyRead();
    void onBinaryMessageReceived(const QByteArray& incomingData);

private:
    QUrl hostAddressToUrl(const QHostAddress& address, int port);
    template<typename... Args>
    static void send(Commands command, Args&&... args);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    explicit BroadcastingManager(QObject* parent = nullptr);
    ~BroadcastingManager() override;

signals:
    void connected();
    void disconnected();
    void terminate();
    void internalTermination();
    void execute(const QString& uid, const QString& projectPath);
    void downloadStarted();
    void downloadProgress(int progress);

private:
    static BroadcastingManager* s_instance;
    static QBasicTimer s_emulatorTimer;
    static QUdpSocket* s_broadcastSocket;
    static QWebSocket* s_webSocket;
    static QTemporaryFile* s_cacheFile;
    static QString s_address;
    static bool s_connected;
};

#endif // BROADCASTINGMANAGER_H