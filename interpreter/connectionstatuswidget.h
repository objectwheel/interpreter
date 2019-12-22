#ifndef CONNECTIONSTATUSWIDGET_H
#define CONNECTIONSTATUSWIDGET_H

#include <QWidget>

class QLabel;
class QVBoxLayout;
class QPropertyAnimation;

class ConnectionStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionStatusWidget(QWidget* parent);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    QSize sizeHint() const override;

public slots:
    void onConnect();
    void onDisconnect();

private:
    QVBoxLayout* m_layout;
    QLabel* m_label;
    QPropertyAnimation* m_animation;

};

#endif // CONNECTIONSTATUSWIDGET_H