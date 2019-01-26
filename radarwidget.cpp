#include <radarwidget.h>
#include <cmath>
#include <QPainter>
#include <QTimerEvent>

RadarWidget::RadarWidget(QWidget* parent) : QWidget(parent)
{
    m_waveAnimation.setLoopCount(-1);
    m_waveAnimation.setStartValue(0.0);
    m_waveAnimation.setEndValue(1.0);
    m_waveAnimation.setEasingCurve(QEasingCurve::OutQuart);

    m_needleAnimation.setLoopCount(-1);
    m_needleAnimation.setStartValue(-0.5 * M_PI);
    m_needleAnimation.setEndValue(3.5 * M_PI);
    m_needleAnimation.setEasingCurve(QEasingCurve::Linear);

    connect(&m_waveAnimation, &QVariantAnimation::stateChanged, this, &RadarWidget::stateChanged);

    start();
}

QAbstractAnimation::State RadarWidget::state() const
{
    return m_waveAnimation.state();
}

int RadarWidget::period() const
{
    return m_waveAnimation.duration() / 2;
}

void RadarWidget::start(int period)
{
    m_needleAnimation.setDuration(2 * period);
    m_waveAnimation.setDuration(2 * period);
    m_needleAnimation.start();
    m_waveAnimation.start();
    m_updateTimer.start(REFRESH_RATE, this);
}

void RadarWidget::stop()
{
    m_updateTimer.stop();
    m_waveAnimation.stop();
    m_needleAnimation.stop();
}

void RadarWidget::pause()
{
    m_updateTimer.stop();
    m_waveAnimation.pause();
    m_needleAnimation.pause();
}

void RadarWidget::resume()
{
    m_needleAnimation.resume();
    m_waveAnimation.resume();
    m_updateTimer.start(REFRESH_RATE, this);
}

void RadarWidget::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_updateTimer.timerId())
        update();
    else
        QWidget::timerEvent(event);
}

void RadarWidget::paintEvent(QPaintEvent*)
{
    QColor waveColor = "#f0f0f0";
    QColor baseColor = "#4ba086";
    QColor frameColor = "#ffffff";
    QColor frameBorderColor = "#c7c7c7"; // #242a2d for light mode
    QColor circleColor = "#50ffffff";
    QColor needleColor = "#ffffff";
    QColor scanningColor = "#77ffd4";

    const QPointF center(QRectF(rect()).center());
    const qreal radius = qMin(width(), height()) / 2.0;
    const qreal waveFactor = m_waveAnimation.currentValue().toReal();
    const qreal needleFactor = m_needleAnimation.currentValue().toReal();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Wave
    waveColor.setAlphaF(1.0 - waveFactor);
    painter.setPen(QPen(waveColor, 1.5));
    painter.drawEllipse(center, waveFactor * radius - 0.5, waveFactor * radius - 0.5);

    // Base
    painter.setPen(Qt::NoPen);
    painter.setBrush(baseColor);
    painter.drawEllipse(center, 64, 64);

    // Frame outer drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#10000000"), 1.5));
    painter.drawEllipse(center + QPointF(0, 1), 64, 64);

    // Frame outer border
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(frameBorderColor, 1));
    painter.drawEllipse(center, 64, 64);

    // Frame
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(frameColor, 1.5));
    painter.drawEllipse(center, 64 - 1, 64 - 1);
    painter.setPen(QPen(frameColor.darker(104), 1.5));
    painter.drawEllipse(center, 64 - 2, 64 - 2);
    painter.setPen(QPen(frameColor.darker(109), 1.5));
    painter.drawEllipse(center, 64 - 3, 64 - 3);
    painter.setPen(QPen(frameColor.darker(115), 1.5));
    painter.drawEllipse(center, 64 - 4, 64 - 4);

    // Circles
    painter.setBrush(Qt::NoBrush);
    painter.setPen(circleColor);
    painter.drawEllipse(center, 45, 45);
    painter.drawEllipse(center, 25, 25);

    // Bubbling circles
    static const qreal begins[] = {
        1.75 * M_PI,
        0.25 * M_PI,
        2.25 * M_PI,
        1.10 * M_PI
    };
    const QPointF points[] = {
        center + QPoint(25, -20),
        center + QPoint(35, 35),
        center + QPoint(35, 35),
        center + QPoint(-35, -5)
    };
    painter.setBrush(Qt::NoBrush);
    for (size_t i = 0; i < sizeof(begins) / sizeof(begins[0]); ++i) {
        static const qreal DIFF = 1.25 * M_PI;
        if (needleFactor > begins[i] && needleFactor < begins[i] + DIFF) {
            const qreal factor = (needleFactor - begins[i]) / DIFF;
            painter.setPen(QColor(255, 255, 255, 255 * (1 - factor)));
            painter.drawEllipse(points[i], factor * 6, factor * 6);
        }
    }

    // Scanning effect
    QConicalGradient scanningEffect(center, - needleFactor * 180 / M_PI);
    scanningColor.setAlpha(140);
    scanningEffect.setColorAt(0.0, scanningColor);
    scanningColor.setAlpha(100);
    scanningEffect.setColorAt(0.2, scanningColor);
    scanningColor.setAlpha(30);
    scanningEffect.setColorAt(0.5, scanningColor);
    scanningColor.setAlpha(5);
    scanningEffect.setColorAt(0.9, scanningColor);
    painter.setBrush(scanningEffect);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 64 - 5, 64 - 5);

    // Needle
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(needleColor, 2.0));
    painter.drawLine(center, center + QPointF(std::cos(needleFactor) * (64 - 6),
                                              std::sin(needleFactor) * (64 - 6)));

    // Needle core drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen("#15000000");
    painter.drawEllipse(center, 8.5, 8.5);

    // Needle core
    QLinearGradient grad(0, 0, 0, 1);
    grad.setCoordinateMode(QGradient::ObjectMode);
    grad.setColorAt(0, needleColor);
    grad.setColorAt(1, needleColor.darker(115));
    painter.setBrush(grad);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 8, 8);

    // Frame inner drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#20000000"), 1.5));
    painter.drawEllipse(center, 64 - 5, 64 - 5);
    painter.setPen(QPen(QColor("#05000000"), 1.5));
    painter.drawEllipse(center, 64 - 6, 64 - 6);
}

QSize RadarWidget::sizeHint() const
{
    return {320, 320};
}
