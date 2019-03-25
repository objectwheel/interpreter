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

protected:
    QSize sizeHint() const override;

private:
    View* m_view;
    ConnectionStatusWidget* m_connectionStatusWidget;
    CentralWidget* m_centralWidget;
};

#endif // APPLICATIONWINDOW_H