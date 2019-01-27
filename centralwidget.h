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
    ConnectivityWidget* m_connectivityWidget;
    QLabel* m_searchingLabel;
};

#endif // CENTRALWIDGET_H