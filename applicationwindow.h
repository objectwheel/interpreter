#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QMainWindow>

class View;
class CentralWidget;
class ConnectionStatusWidget;

class ApplicationWindow : public QMainWindow
{
    Q_OBJECT

    enum {
        Central
    };

public:
    explicit ApplicationWindow(QWidget* parent = nullptr);
    CentralWidget* centralWidget() const;
    QSize sizeHint() const override;

public slots:
    bool mightThemeChange(const QString& uid);

private:
    View* m_view;
    ConnectionStatusWidget* m_connectionStatusWidget;
    CentralWidget* m_centralWidget;
};

#endif // APPLICATIONWINDOW_H