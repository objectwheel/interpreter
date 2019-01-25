#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QMainWindow>

class View;
class CentralWidget;

class ApplicationWindow : public QMainWindow
{
    Q_OBJECT

    enum {
        Central
    };

public:
    explicit ApplicationWindow(QWidget* parent = nullptr);

protected:
    QSize sizeHint() const override;

private:
    View* m_view;
    CentralWidget* m_centralWidget;
};

#endif // APPLICATIONWINDOW_H