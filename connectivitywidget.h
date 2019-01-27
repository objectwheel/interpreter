#ifndef CONNECTIVITYWIDGET_H
#define CONNECTIVITYWIDGET_H

#include <QWidget>
#include <QVariantAnimation>

class RadarWidget;
class QGraphicsOpacityEffect;

class ConnectivityWidget final : public QWidget
{
    Q_OBJECT

public:
    enum Status {
        Connected,
        Searching,
        Disabled
    };

public:
    explicit ConnectivityWidget(QWidget* parent = nullptr);

    Status status() const;
    void setStatus(Status status);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    Status m_status;
    RadarWidget* m_radarWidget;
    QGraphicsOpacityEffect* m_opacityEffect;
    QVariantAnimation m_opacityAnimation;

};

#endif // CONNECTIVITYWIDGET_H