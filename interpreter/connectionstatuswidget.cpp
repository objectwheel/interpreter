#include <connectionstatuswidget.h>
#include <broadcastingmanager.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>

ConnectionStatusWidget::ConnectionStatusWidget(QWidget* parent) : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_label(new QLabel(this))
  , m_animation(new QPropertyAnimation(this, "pos"))
{
    resize(sizeHint());
    setAutoFillBackground(true);
    parent->installEventFilter(this);

    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(8);
    m_layout->addWidget(m_label, 0, Qt::AlignCenter);

    connect(BroadcastingManager::instance(), &BroadcastingManager::connected,
            this, &ConnectionStatusWidget::onConnect);
    connect(BroadcastingManager::instance(), &BroadcastingManager::disconnected,
            this, &ConnectionStatusWidget::onDisconnect);

    m_animation->setDuration(600);
    m_animation->setEasingCurve(QEasingCurve::OutExpo);
    move(0, -height());
}

bool ConnectionStatusWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == qobject_cast<QObject*>(parentWidget()) && event->type() == QEvent::Resize) {
        setGeometry(0, y(), parentWidget()->width(), height());
        return false;
    } else {
        return QWidget::eventFilter(watched, event);
    }
}

QSize ConnectionStatusWidget::sizeHint() const
{
    return {30, 30};
}

void ConnectionStatusWidget::onConnect()
{
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, "#16a085");
    setPalette(palette);

    m_label->setText(tr("Connected to ") + BroadcastingManager::address());

    if (y() != 0) {
        if (m_animation->state() == QAbstractAnimation::Running) {
            m_animation->stop();
            m_animation->setStartValue(QPoint(0, y()));
        } else
            m_animation->setStartValue(QPoint(0, -height()));
#if defined(Q_OS_IOS)
        m_animation->setEndValue(QPoint(0, 20));
#else
        m_animation->setEndValue(QPoint(0, 0));
#endif
        m_animation->start();
    }
}

void ConnectionStatusWidget::onDisconnect()
{
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, "#c0392b");
    setPalette(palette);

    m_label->setText(tr("Disconnected from ") + BroadcastingManager::address());

    static QTimer timer;
    static auto connection = connect(&timer, &QTimer::timeout, this, [=] {
        timer.stop();
        if (!BroadcastingManager::isConnected()) {
#if defined(Q_OS_IOS)
            m_animation->setStartValue(QPoint(0, 20));
#else
            m_animation->setStartValue(QPoint(0, 0));
#endif
            m_animation->setEndValue(QPoint(0, -height()));
            m_animation->start();
        }
    });
    Q_UNUSED(connection)
    timer.start(5000);
}
