#include <connectivitywidget.h>
#include <radarwidget.h>

#include <QSvgWidget>
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QTimerEvent>
#include <QSvgRenderer>
#include <QLabel>

ConnectivityWidget::ConnectivityWidget(QWidget* parent) : QWidget(parent)
  , m_state(Disabled)
  , m_oldState(m_state)
  , m_disabledWidget(new QSvgWidget(this))
  , m_connectedWidget(new QSvgWidget(this))
  , m_searchingWidget(new RadarWidget(this))
  , m_statusLabel(new QLabel(this))
{
    QPalette palette(m_searchingWidget->palette());
    palette.setColor(QPalette::Link, "#50e0e4e7");        // Wave color
    palette.setColor(QPalette::Base, "#2980b9");          // Base color
    palette.setColor(QPalette::Button, "#3C454C");        // Frame color
    palette.setColor(QPalette::ButtonText, "#242a2d");    // Frame border color
    palette.setColor(QPalette::Highlight, "#e0e4e7");     // Bubble color
    palette.setColor(QPalette::LinkVisited, "#50e0e4e7"); // Circle color
    palette.setColor(QPalette::ToolTipBase, "#e0e4e7");   // Needle color
    palette.setColor(QPalette::ToolTipText, "#38afff");   // Scanning color
    m_searchingWidget->setPalette(palette);

    QFont font;
    font.setPixelSize(13);
    font.setWeight(QFont::Light);
    m_statusLabel->setFont(font);
    m_statusLabel->setText(tr("Disabled"));
    m_statusLabel->setAlignment(Qt::AlignHCenter);
    m_statusLabel->resize(sizeHint().width(), 18);

    auto disabledOpacityEffect = new QGraphicsOpacityEffect(m_disabledWidget);
    disabledOpacityEffect->setOpacity(1.0);
    m_disabledWidget->show();
    m_disabledWidget->resize(m_searchingWidget->minimumSizeHint());
    m_disabledWidget->load(QStringLiteral(":/images/disconnected.svg"));
    m_disabledWidget->setGraphicsEffect(disabledOpacityEffect);

    auto connectedOpacityEffect = new QGraphicsOpacityEffect(m_connectedWidget);
    connectedOpacityEffect->setOpacity(0.0);
    m_connectedWidget->hide();
    m_connectedWidget->resize(m_searchingWidget->minimumSizeHint());
    m_connectedWidget->load(QStringLiteral(":/images/connected.svg"));
    m_connectedWidget->setGraphicsEffect(connectedOpacityEffect);
    m_connectedWidget->renderer()->setFramesPerSecond(30);

    auto searchingOpacityEffect = new QGraphicsOpacityEffect(m_searchingWidget);
    searchingOpacityEffect->setOpacity(0.0);
    m_searchingWidget->hide();
    m_searchingWidget->start(1500);
    m_searchingWidget->resize(m_searchingWidget->sizeHint());
    m_searchingWidget->setGraphicsEffect(searchingOpacityEffect);

    m_opacityAnimation.setDuration(500);
    m_opacityAnimation.setStartValue(0.0);
    m_opacityAnimation.setEndValue(1.0);
    m_opacityAnimation.setEasingCurve(QEasingCurve::OutQuart);
    connect(&m_opacityAnimation, &QVariantAnimation::valueChanged,
            this, &ConnectivityWidget::onAnimationValueChange);
}

ConnectivityWidget::State ConnectivityWidget::state() const
{
    return m_state;
}

void ConnectivityWidget::setState(ConnectivityWidget::State state)
{
    State tooOldState = m_oldState;
    m_oldState = m_state;
    m_state = state;

    if (m_state == Disabled)
        m_statusLabel->setText(tr("Disabled"));
    else if (m_state == Connected)
        m_statusLabel->setText(tr("Ready"));
    else if (m_state == Searching)
        m_statusLabel->setText(tr("Searching..."));

    if (m_state == m_oldState)
        return;

    stateWidget(m_state)->show();

    if (m_opacityAnimation.state() == QAbstractAnimation::Running) {
        if (m_state != tooOldState) {
            QWidget* tooOldWidget = stateWidget(tooOldState);
            auto tooOldGraphicsEffect = opacityEffect(tooOldWidget);
            tooOldWidget->hide();
            tooOldGraphicsEffect->setOpacity(0.0);
        }
    } else {
        m_opacityAnimation.start();
    }
}

void ConnectivityWidget::onAnimationValueChange(const QVariant& value)
{
    QWidget* newWidget = stateWidget(m_state);
    QWidget* oldWidget = stateWidget(m_oldState);

    auto newGraphicsEffect = opacityEffect(newWidget);
    auto oldGraphicsEffect = opacityEffect(oldWidget);

    qreal opacity = value.toReal();
    newGraphicsEffect->setOpacity(opacity);
    oldGraphicsEffect->setOpacity(1 - opacity);

    if (opacity == 1.0)
        oldWidget->hide();
}

QWidget* ConnectivityWidget::stateWidget(ConnectivityWidget::State state) const
{
    if (state == Disabled)
        return m_disabledWidget;
    if (state == Connected)
        return m_connectedWidget;
    return m_searchingWidget;
}

QGraphicsOpacityEffect* ConnectivityWidget::opacityEffect(const QWidget* widget) const
{
    return qobject_cast<QGraphicsOpacityEffect*>(widget->graphicsEffect());
}

void ConnectivityWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    m_searchingWidget->setGeometry(this->rect());
    m_statusLabel->resize(width(), m_statusLabel->height());
    m_statusLabel->move(0, height() / 2 + m_searchingWidget->minimumSizeHint().height() / 2 +
                        m_statusLabel->height() / 2);

    QRect rect;
    rect = m_disabledWidget->rect();
    rect.moveCenter(this->rect().center());
    m_disabledWidget->move(rect.topLeft());

    rect = m_connectedWidget->rect();
    rect.moveCenter(this->rect().center());
    m_connectedWidget->move(rect.topLeft());
}

QSize ConnectivityWidget::sizeHint() const
{
    return m_searchingWidget->sizeHint();
}

QSize ConnectivityWidget::minimumSizeHint() const
{
    return m_searchingWidget->minimumSizeHint() + QSize(0, m_statusLabel->height() * 3);
}
