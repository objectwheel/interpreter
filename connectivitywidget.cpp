#include <connectivitywidget.h>
#include <radarwidget.h>

#include <QSvgWidget>
#include <QPainter>
#include <QGraphicsOpacityEffect>

#include <QTimer>

ConnectivityWidget::ConnectivityWidget(QWidget* parent) : QWidget(parent)
  , m_state(Connected)
  , m_oldState(Connected)
  , m_disabledWidget(new QSvgWidget(this))
  , m_connectedWidget(new QSvgWidget(this))
  , m_searchingWidget(new RadarWidget(this))
  , m_opacityEffect(new QGraphicsOpacityEffect(this))
{
    QPalette palette(m_searchingWidget->palette());
    palette.setColor(QPalette::Link, "#803382B5");        // Wave color
    palette.setColor(QPalette::Base, "#3382B5");          // Base color
    palette.setColor(QPalette::Button, "#ffffff");        // Frame color
    palette.setColor(QPalette::ButtonText, "#242a2d");    // Frame border color
    palette.setColor(QPalette::LinkVisited, "#50ffffff"); // Circle color
    palette.setColor(QPalette::ToolTipBase, "#ffffff");   // Needle color
    palette.setColor(QPalette::ToolTipText, "#85cfff");   // Scanning color

    m_disabledWidget->hide();
    m_disabledWidget->resize(128, 128);
    m_disabledWidget->load(QStringLiteral(":/images/disconnected.svg"));

    m_connectedWidget->hide();
    m_connectedWidget->resize(128, 128);
    m_connectedWidget->load(QStringLiteral(":/images/connected.svg"));

    m_searchingWidget->hide();
    m_searchingWidget->start();
    m_searchingWidget->setPalette(palette);
    m_searchingWidget->setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(0.0);

    m_opacityAnimation.setDuration(600);
    m_opacityAnimation.setStartValue(0.0);
    m_opacityAnimation.setEndValue(1.0);
    m_opacityAnimation.setEasingCurve(QEasingCurve::OutQuart);
    connect(&m_opacityAnimation, &QVariantAnimation::valueChanged,
            this, &ConnectivityWidget::onAnimationValueChange);

    QTimer::singleShot(2000, [=] { setState(m_state); });
}

ConnectivityWidget::State ConnectivityWidget::state() const
{
    return m_state;
}

void ConnectivityWidget::setState(ConnectivityWidget::State state)
{
    m_oldState = m_state;
    m_state = state;

    if (state == Searching)
        m_searchingWidget->show();

    m_opacityAnimation.start();
}

void ConnectivityWidget::onAnimationValueChange(const QVariant& value)
{
    qreal opacity = value.toReal();
    if (m_state == Searching) {
        m_opacityEffect->setOpacity(opacity);
    } else if (m_oldState == Searching) {
        m_opacityEffect->setOpacity(1 - opacity);
        if (opacity == 1.0)
            m_searchingWidget->hide();
    }
}

void ConnectivityWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    QRect rect;
    rect = m_disabledWidget->rect();
    rect.moveCenter(this->rect().center());
    m_disabledWidget->move(rect.topLeft());
    rect = m_connectedWidget->rect();
    rect.moveCenter(this->rect().center());
    m_connectedWidget->move(rect.topLeft());
    rect = m_searchingWidget->rect();
    rect.moveCenter(this->rect().center());
    m_searchingWidget->move(rect.topLeft());
}

QSize ConnectivityWidget::sizeHint() const
{
    return {300, 300};
}

#include "connectivitywidget.moc"