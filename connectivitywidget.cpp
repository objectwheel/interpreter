#include <connectivitywidget.h>
#include <radarwidget.h>

#include <QPainter>
#include <QGraphicsOpacityEffect>

namespace { ConnectivityWidget::State g_previousState; }

ConnectivityWidget::ConnectivityWidget(QWidget* parent) : QWidget(parent)
  , m_state(Searching)
  , m_radarWidget(new RadarWidget(this))
  , m_opacityEffect(new QGraphicsOpacityEffect(this))
{
    QPalette palette(m_radarWidget->palette());
    palette.setColor(QPalette::Link, "#803382B5");        // Wave color
    palette.setColor(QPalette::Base, "#3382B5");          // Base color
    palette.setColor(QPalette::Button, "#ffffff");        // Frame color
    palette.setColor(QPalette::ButtonText, "#242a2d");    // Frame border color
    palette.setColor(QPalette::LinkVisited, "#50ffffff"); // Circle color
    palette.setColor(QPalette::ToolTipBase, "#ffffff");   // Needle color
    palette.setColor(QPalette::ToolTipText, "#85cfff");   // Scanning color
    m_radarWidget->setPalette(palette);

    m_opacityEffect->setOpacity(0.0);
    m_radarWidget->hide();
    m_radarWidget->start();
    m_radarWidget->setGraphicsEffect(m_opacityEffect);

    m_opacityAnimation.setDuration(600);
    m_opacityAnimation.setStartValue(0.0);
    m_opacityAnimation.setEndValue(1.0);
    m_opacityAnimation.setEasingCurve(QEasingCurve::OutQuart);

    setState(m_state);
}

ConnectivityWidget::State ConnectivityWidget::state() const
{
    return m_state;
}

void ConnectivityWidget::setState(ConnectivityWidget::State state)
{
    g_previousState = m_state;
    m_state = state;

    if (state == Searching)
        m_radarWidget->show();

    m_opacityAnimation.start();
}

void ConnectivityWidget::onAnimationValueChange(const QVariant& value)
{
    qreal opacity = value.toReal();
    if (m_state == Searching) {
        m_opacityEffect->setOpacity(opacity);
    } else if (g_previousState == Searching) {
        m_opacityEffect->setOpacity(1 - opacity);
        if (opacity == 1.0)
            m_radarWidget->hide();
    }
}

void ConnectivityWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    m_radarWidget->setGeometry(rect());
}

void ConnectivityWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch (m_state) {
    case Connected: {
        static QPixmap icon(":/images/connected.svg");
        painter.drawPixmap(rect(), icon);
    } break;
    case Disabled: {
        static QPixmap icon(":/images/disconnected.svg");
        painter.drawPixmap(rect(), icon);
    } break;
    default:
        break;
    }
}

QSize ConnectivityWidget::sizeHint() const
{
    return {300, 300};
}