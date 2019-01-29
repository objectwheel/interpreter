#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QVBoxLayout;
class ConnectivityWidget;
class QLabel;
class QPushButton;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget* parent = nullptr);

private slots:
    void onDisableDiscoveryButtonClicked();

private:
    QVBoxLayout* m_layout;
    QVBoxLayout* m_buttonLayout;
    ConnectivityWidget* m_connectivityWidget;
    QLabel* m_titleLabel;
    QPushButton* m_disableDiscoveryButton;
    QPushButton* m_connectManuallyButton;
    QPushButton* m_myProjectsButton;
    QPushButton* m_settingsButton;
};

#endif // CENTRALWIDGET_H