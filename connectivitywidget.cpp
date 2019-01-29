#include <connectivitywidget.h>
#include <radarwidget.h>

#include <QSvgWidget>
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QTimerEvent>
#include <QSvgRenderer>
#include <QLabel>

ConnectivityWidget::ConnectivityWidget(QWidget* parent) : QWidget(parent)
  , m_state(Stopped)
  , m_oldState(m_state)
  , m_stoppedWidget(new QSvgWidget(this))
  , m_connectedWidget(new QSvgWidget(this))
  , m_searchingWidget(new RadarWidget(this))
  , m_statusLabel(new QLabel(this))
{
    QPalette palette(m_searchingWidget->palette());
    palette.setColor(QPalette::Link, "#50ffffff");        // Wave color
    palette.setColor(QPalette::Base, "#3382B5");          // Base color
    palette.setColor(QPalette::Button, "#ffffff");        // Frame color
    palette.setColor(QPalette::ButtonText, "#242a2d");    // Frame border color
    palette.setColor(QPalette::LinkVisited, "#50ffffff"); // Circle color
    palette.setColor(QPalette::ToolTipBase, "#ffffff");   // Needle color
    palette.setColor(QPalette::ToolTipText, "#85cfff");   // Scanning color
    m_searchingWidget->setPalette(palette);

    QFont font;
    font.setPixelSize(13);
    font.setWeight(QFont::Light);
    m_statusLabel->setFont(font);
    m_statusLabel->setText(tr("Stopped"));
    m_statusLabel->setAlignment(Qt::AlignHCenter);
    m_statusLabel->resize(sizeHint().width(), 18);

    auto stoppedOpacityEffect = new QGraphicsOpacityEffect(m_stoppedWidget);
    stoppedOpacityEffect->setOpacity(1.0);
    m_stoppedWidget->show();
    m_stoppedWidget->resize(128, 128);
    m_stoppedWidget->load(QStringLiteral(":/images/disconnected.svg"));
    m_stoppedWidget->setGraphicsEffect(stoppedOpacityEffect);

    auto connectedOpacityEffect = new QGraphicsOpacityEffect(m_connectedWidget);
    connectedOpacityEffect->setOpacity(0.0);
    m_connectedWidget->hide();
    m_connectedWidget->resize(128, 128);
    m_connectedWidget->load(QStringLiteral(":/images/connected.svg"));
    m_connectedWidget->setGraphicsEffect(connectedOpacityEffect);
    m_connectedWidget->renderer()->setFramesPerSecond(30);

    auto searchingOpacityEffect = new QGraphicsOpacityEffect(m_searchingWidget);
    searchingOpacityEffect->setOpacity(0.0);
    m_searchingWidget->hide();
    m_searchingWidget->start();
    m_searchingWidget->resize(300, 300);
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

    if (m_state == Stopped)
        m_statusLabel->setText(tr("Stopped"));
    else if (m_state == Connected)
        m_statusLabel->setText(tr("Ready"));
    else if (m_state == Searching)
        m_statusLabel->setText(tr("Searching..."));

    if (m_state == m_oldState)
        return;

    stateWidget(m_state)->show();

    if (m_opacityAnimation.state() == QAbstractAnimation::Running) {
        QWidget* tooOldWidget = stateWidget(tooOldState);
        auto tooOldGraphicsEffect = qobject_cast<QGraphicsOpacityEffect*>(tooOldWidget->graphicsEffect());

        tooOldWidget->hide();
        tooOldGraphicsEffect->setOpacity(0.0);
    } else {
        m_opacityAnimation.start();
    }
}

void ConnectivityWidget::onAnimationValueChange(const QVariant& value)
{
    QWidget* newWidget = stateWidget(m_state);
    QWidget* oldWidget = stateWidget(m_oldState);

    auto newGraphicsEffect = qobject_cast<QGraphicsOpacityEffect*>(newWidget->graphicsEffect());
    auto oldGraphicsEffect = qobject_cast<QGraphicsOpacityEffect*>(oldWidget->graphicsEffect());

    qreal opacity = value.toReal();
    newGraphicsEffect->setOpacity(opacity);
    oldGraphicsEffect->setOpacity(1 - opacity);

    if (opacity == 1.0)
        oldWidget->hide();
}

QWidget* ConnectivityWidget::stateWidget(ConnectivityWidget::State state) const
{
    if (state == Stopped)
        return m_stoppedWidget;
    if (state == Connected)
        return m_connectedWidget;
    return m_searchingWidget;
}

void ConnectivityWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    QRect rect(0, 230, width(), m_statusLabel->height());
    m_statusLabel->move(rect.topLeft());

    rect = m_stoppedWidget->rect();
    rect.moveCenter(this->rect().center());
    m_stoppedWidget->move(rect.topLeft());

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
