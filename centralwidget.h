#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class ConnectivityWidget;
class QLabel;
class QPushButton;
class QScrollArea;
class QFrame;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget* parent = nullptr);

signals:
    void infoButtonClicked();
    void quitButtonClicked();
    void settingsButtonClicked();
    void myProjectsButtonClicked();
    void connectManuallyButtonClicked();
    void disableDiscoveryButtonClicked(bool disabled);

private:
    QVBoxLayout* m_layout;
    ConnectivityWidget* m_connectivityWidget;
    QFrame* m_line;
    QLabel* m_titleLabel;
    QLabel* m_versionLabel;
    QHBoxLayout* m_scrollAreaLayout;
    QScrollArea* m_buttonsScrollArea;
    QVBoxLayout* m_buttonLayout;
    QPushButton* m_disableDiscoveryButton;
    QPushButton* m_connectManuallyButton;
    QPushButton* m_myProjectsButton;
    QPushButton* m_settingsButton;
    QPushButton* m_quitButton;
    QPushButton* m_infoButton;
};

#endif // CENTRALWIDGET_H