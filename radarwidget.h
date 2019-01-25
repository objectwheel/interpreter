#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>
#include <QVariantAnimation>

class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget* parent = nullptr);

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent* event) override;

private:
    QVariantAnimation m_circleRadiusAnimation;
    QVariantAnimation m_needleAnimation;
};

#endif // RADARWIDGET_H