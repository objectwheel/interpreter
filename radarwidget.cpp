#include <radarwidget.h>
#include <QtMath>
#include <QPainter>
#include <QTimerEvent>

namespace {

bool g_hidden = false;
QAbstractAnimation::State g_previousState = QAbstractAnimation::Stopped;
}

RadarWidget::RadarWidget(QWidget* parent) : QWidget(parent)
  , m_scaleFactor(1.0)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QPalette palette(this->palette());
    palette.setColor(QPalette::Link, "#755EC8A7");        // Wave color
    palette.setColor(QPalette::Base, "#4ba086");          // Base color
    palette.setColor(QPalette::Button, "#ffffff");        // Frame color
    palette.setColor(QPalette::ButtonText, "#242a2d");    // Frame border color
    palette.setColor(QPalette::LinkVisited, "#50ffffff"); // Circle color
    palette.setColor(QPalette::ToolTipBase, "#ffffff");   // Needle color
    palette.setColor(QPalette::ToolTipText, "#77ffd4");   // Scanning color
    setPalette(palette);

    m_waveAnimation.setLoopCount(-1);
    m_waveAnimation.setStartValue(0.0);
    m_waveAnimation.setEndValue(1.0);
    m_waveAnimation.setEasingCurve(QEasingCurve::OutQuart);

    m_needleAnimation.setLoopCount(-1);
    m_needleAnimation.setStartValue(-0.5 * M_PI);
    m_needleAnimation.setEndValue(3.5 * M_PI);
    m_needleAnimation.setEasingCurve(QEasingCurve::Linear);

    start();
}

QAbstractAnimation::State RadarWidget::state() const
{
    if (g_hidden)
        return g_previousState;
    return m_waveAnimation.state();
}

int RadarWidget::period() const
{
    return m_waveAnimation.duration() / 2;
}

qreal RadarWidget::scaleFactor() const
{
    return m_scaleFactor;
}

void RadarWidget::setScaleFactor(qreal scaleFactor)
{
    m_scaleFactor = scaleFactor;
    update();
}

void RadarWidget::start(int period)
{
    QAbstractAnimation::State oldState = state();
    if (!g_hidden) {
        m_needleAnimation.setDuration(2 * period);
        m_waveAnimation.setDuration(2 * period);
        m_needleAnimation.start();
        m_waveAnimation.start();
        m_updateTimer.start(REFRESH_RATE, this);
    }
    g_previousState = state();
    stateChanged(g_previousState, oldState);
}

void RadarWidget::stop()
{
    QAbstractAnimation::State oldState = state();
    m_updateTimer.stop();
    m_waveAnimation.stop();
    m_needleAnimation.stop();
    g_previousState = state();
    stateChanged(g_previousState, oldState);
}

void RadarWidget::pause()
{
    QAbstractAnimation::State oldState = state();
    m_updateTimer.stop();
    m_waveAnimation.pause();
    m_needleAnimation.pause();
    g_previousState = state();
    stateChanged(g_previousState, oldState);
}

void RadarWidget::resume()
{
    QAbstractAnimation::State oldState = state();
    if (!g_hidden) {
        m_needleAnimation.resume();
        m_waveAnimation.resume();
        m_updateTimer.start(REFRESH_RATE, this);
    }
    g_previousState = state();
    stateChanged(g_previousState, oldState);
}

void RadarWidget::hideEvent(QHideEvent* event)
{
    g_previousState = state();
    g_hidden = true;
    if (g_previousState == QAbstractAnimation::Running) {
        m_updateTimer.stop();
        m_waveAnimation.pause();
        m_needleAnimation.pause();
    }
    QWidget::hideEvent(event);
}

void RadarWidget::showEvent(QShowEvent* event)
{
    if (g_previousState == QAbstractAnimation::Running && g_hidden) {
        m_needleAnimation.resume();
        m_waveAnimation.resume();
        m_updateTimer.start(REFRESH_RATE, this);
    }
    g_hidden = false;
    QWidget::showEvent(event);
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
    const QPointF center(QRectF(rect()).center());
    const qreal sf = m_scaleFactor;
    const qreal radius = qMin(width(), height()) / 2.0;
    const qreal waveFactor = m_waveAnimation.currentValue().toReal();
    const qreal needleFactor = m_needleAnimation.currentValue().toReal();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Wave
    QColor waveColor = palette().link().color();
    waveColor.setAlphaF(waveColor.alphaF() * (1 - waveFactor));
    painter.setPen(QPen(waveColor, 1.5 * sf));
    painter.drawEllipse(center, waveFactor * radius - 0.5 * sf, waveFactor * radius - 0.5 * sf);

    // Base
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().base());
    painter.drawEllipse(center, FRAME_RADIUS * sf, FRAME_RADIUS * sf);

    // Frame outer drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#10000000"), 1.5 * sf));
    painter.drawEllipse(center + QPointF(0, 1 * sf), FRAME_RADIUS * sf, FRAME_RADIUS * sf);

    // Frame outer border
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(palette().buttonText(), 1 * sf));
    painter.drawEllipse(center, FRAME_RADIUS * sf, FRAME_RADIUS * sf);

    // Frame
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(palette().button(), 1.5 * sf));
    painter.drawEllipse(center, FRAME_RADIUS * sf - 1 * sf, FRAME_RADIUS * sf - 1 * sf);
    painter.setPen(QPen(palette().button().color().darker(104), 1.5 * sf));
    painter.drawEllipse(center, FRAME_RADIUS * sf - 2 * sf, FRAME_RADIUS * sf - 2 * sf);
    painter.setPen(QPen(palette().button().color().darker(109), 1.5 * sf));
    painter.drawEllipse(center, FRAME_RADIUS * sf - 3 * sf, FRAME_RADIUS * sf - 3 * sf);
    painter.setPen(QPen(palette().button().color().darker(115), 1.5 * sf));
    painter.drawEllipse(center, FRAME_RADIUS * sf - 4 * sf, FRAME_RADIUS * sf - 4 * sf);

    // Circles
    painter.setBrush(Qt::NoBrush);
    painter.setPen(palette().linkVisited().color());
    painter.drawEllipse(center, 45 * sf, 45 * sf);
    painter.drawEllipse(center, 25 * sf, 25 * sf);

    // Bubbling circles
    static const qreal begins[] = {
        1.75 * M_PI,
        0.25 * M_PI,
        2.25 * M_PI,
        1.10 * M_PI
    };
    const QPointF points[] = {
        center + QPoint(25 * sf, -20 * sf),
        center + QPoint(35 * sf, 35 * sf),
        center + QPoint(35 * sf, 35 * sf),
        center + QPoint(-35 * sf, -5 * sf)
    };
    painter.setBrush(Qt::NoBrush);
    for (size_t i = 0; i < sizeof(begins) / sizeof(begins[0]); ++i) {
        static const qreal DIFF = 1.25 * M_PI;
        if (needleFactor > begins[i] && needleFactor < begins[i] + DIFF) {
            const qreal factor = (needleFactor - begins[i]) / DIFF;
            painter.setPen(QColor(255, 255, 255, 255 * (1 - factor)));
            painter.drawEllipse(points[i], factor * 6 * sf, factor * 6 * sf);
        }
    }

    // Scanning effect
    QColor scanningColor = palette().toolTipText().color();
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
    painter.drawEllipse(center, FRAME_RADIUS * sf - 5 * sf, FRAME_RADIUS * sf - 5 * sf);

    // Needle
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(palette().toolTipBase(), 2.0 * sf));
    painter.drawLine(center, center + QPointF(std::cos(needleFactor) * (FRAME_RADIUS * sf - 6 * sf),
                                              std::sin(needleFactor) * (FRAME_RADIUS * sf - 6 * sf)));

    // Needle core drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen("#15000000");
    painter.drawEllipse(center, 8.5 * sf, 8.5 * sf);

    // Needle core
    QLinearGradient grad(0, 0, 0, 1);
    grad.setCoordinateMode(QGradient::ObjectMode);
    grad.setColorAt(0, palette().toolTipBase().color());
    grad.setColorAt(1, palette().toolTipBase().color().darker(115));
    painter.setBrush(grad);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 8 * sf, 8 * sf);

    // Frame inner drop shadow
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#20000000"), 1.5 * sf));
    painter.drawEllipse(center, FRAME_RADIUS * sf - 5 * sf, FRAME_RADIUS * sf - 5 * sf);
    painter.setPen(QPen(QColor("#05000000"), 1.5 * sf));
    painter.drawEllipse(center, FRAME_RADIUS * sf - 6 * sf, FRAME_RADIUS * sf - 6 * sf);
}

QSize RadarWidget::sizeHint() const
{
    return {300, 300};
}

QSize RadarWidget::minimumSizeHint() const
{
    return {
        qCeil(2 * FRAME_RADIUS * m_scaleFactor) + 1,
        qCeil(2 * FRAME_RADIUS * m_scaleFactor) + 1
    };
}
