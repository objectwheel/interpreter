#include <connectivitywidget.h>
#include <radarwidget.h>

#include <QPainter>
#include <QGraphicsOpacityEffect>

ConnectivityWidget::ConnectivityWidget(QWidget* parent) : QWidget(parent)
  , m_status(Searching)
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
}

ConnectivityWidget::Status ConnectivityWidget::status() const
{
    return m_status;
}

void ConnectivityWidget::setStatus(ConnectivityWidget::Status status)
{
    bool changed = m_status != status;
    m_status = status;
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

    switch (m_status) {
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
