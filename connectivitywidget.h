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
    enum State {
        Connected,
        Searching,
        Disabled
    };

public:
    explicit ConnectivityWidget(QWidget* parent = nullptr);

    State state() const;
    void setState(State state);

private slots:
    void onAnimationValueChange(const QVariant& value);

protected:
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    State m_state;
    RadarWidget* m_radarWidget;
    QGraphicsOpacityEffect* m_opacityEffect;
    QVariantAnimation m_opacityAnimation;

};

#endif // CONNECTIVITYWIDGET_H