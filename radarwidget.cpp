#include <radarwidget.h>
#include <QPainter>
#include <QTimerEvent>
#include <cmath>

RadarWidget::RadarWidget(QWidget* parent) : QWidget(parent)
{
    m_circleRadiusAnimation.setStartValue(0.3);
    m_circleRadiusAnimation.setEndValue(1.0);
    m_circleRadiusAnimation.setLoopCount(-1);
    m_circleRadiusAnimation.setDuration(3000);
    m_circleRadiusAnimation.setEasingCurve(QEasingCurve::OutQuart);
    m_circleRadiusAnimation.start();
//    connect(&m_circleRadiusAnimation, &QVariantAnimation::valueChanged,
//            this, QOverload<>::of(&RadarWidget::update));

    m_needleAnimation.setStartValue(0.0);
    m_needleAnimation.setEndValue(2 * M_PI);
    m_needleAnimation.setLoopCount(-1);
    m_needleAnimation.setDuration(2000);
    m_needleAnimation.setEasingCurve(QEasingCurve::Linear);
    m_needleAnimation.start();
    connect(&m_needleAnimation, &QVariantAnimation::valueChanged,
            this, QOverload<>::of(&RadarWidget::update));
}

QSize RadarWidget::sizeHint() const
{
    return {320, 320};
}

void RadarWidget::paintEvent(QPaintEvent*)
{
    QPointF center(QRectF(rect()).center());
    const qreal circleRadiusFactor = m_circleRadiusAnimation.currentValue().toReal();
    const qreal length = qMin(width(), height());
    const qreal circleRadius = circleRadiusFactor * length / 2.0 - 0.75;
    QColor circleBorderColor(palette().windowText().color());
    circleBorderColor.setAlphaF(1 - circleRadiusFactor);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(circleBorderColor, 1.5));
    painter.drawEllipse(center, circleRadius, circleRadius);

    // Draw core's background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#2683E1"));
    painter.drawEllipse(center, length / 5.0, length / 5.0);

    // Draw core's border
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#ffffff"), 1.5));
    painter.drawEllipse(center, length / 5.0, length / 5.0);
    painter.setPen(QPen(QColor("#f2f2f2"), 1.5));
    painter.drawEllipse(center, length / 5.0 - 1, length / 5.0 - 1);
    painter.setPen(QPen(QColor("#e8e8e8"), 1.5));
    painter.drawEllipse(center, length / 5.0 - 2, length / 5.0 - 2);
    painter.setPen(QPen(QColor("#e0e0e0"), 1.5));
    painter.drawEllipse(center, length / 5.0 - 3, length / 5.0 - 3);

    // Draw core's inner lines
    painter.setPen("#30ffffff");
    painter.drawEllipse(center, length / 7.0, length / 7.0);
    painter.drawEllipse(center, length / 12.0, length / 12.0);

    // Draw core's needle
    painter.setPen(QPen(QColor("white"), 2.0));
    painter.drawLine(center, center +
                     QPointF(std::cos(m_needleAnimation.currentValue().toReal()) * (length / 5.0 - 5),
                             std::sin(m_needleAnimation.currentValue().toReal()) * (length / 5.0 - 5)));

    QConicalGradient grad2(center, -m_needleAnimation.currentValue().toReal() * 180 / M_PI);
    grad2.setColorAt(0.0, "#50ffffff");
    grad2.setColorAt(0.2, "#40ffffff");
    grad2.setColorAt(0.8, "#05ffffff");
    grad2.setColorAt(1.0, "#00ffffff");
    painter.setBrush(grad2);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, length / 5.0 - 4, length / 5.0 - 4);

    // Draw core's border drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#35000000"), 1.5));
    painter.drawEllipse(center, length / 5.0 - 4, length / 5.0 - 4);
    painter.setPen(QPen(QColor("#15000000"), 1.5));
    painter.drawEllipse(center, length / 5.0 - 5, length / 5.0 - 5);
    painter.setPen(QPen(QColor("#07000000"), 1.5));
    painter.drawEllipse(center, length / 5.0 - 5, length / 5.0 - 5);

    // Draw core's needle ball
    QLinearGradient grad(0, 0, 0, 1);
    grad.setCoordinateMode(QGradient::ObjectMode);
    grad.setColorAt(0, "#ffffff");
    grad.setColorAt(1, "#e0e0e0");
    painter.setBrush(grad);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 8, 8);

    // Draw core's needle ball drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen("#20000000");
    painter.drawEllipse(center, 8.5, 8.5);
}
