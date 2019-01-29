#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QVBoxLayout;
class ConnectivityWidget;
class QLabel;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget* parent = nullptr);

private:
    QVBoxLayout* m_layout;
    QVBoxLayout* m_buttonLayout;
    ConnectivityWidget* m_connectivityWidget;
    QLabel* m_titleLabel;
};

#endif // CENTRALWIDGET_H