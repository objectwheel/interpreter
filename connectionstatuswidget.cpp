#include <connectionstatuswidget.h>
#include <discoverymanager.h>

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
    setAutoFillBackground(true);
    parent->installEventFilter(this);

    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(8);
    m_layout->addWidget(m_label, 0, Qt::AlignCenter);

    connect(DiscoveryManager::instance(), &DiscoveryManager::connected,
            this, &ConnectionStatusWidget::onConnect);
    connect(DiscoveryManager::instance(), &DiscoveryManager::disconnected,
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
    return {25, 25};
}

void ConnectionStatusWidget::onConnect()
{
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, "#4ba086");
    setPalette(palette);

    m_label->setText(tr("Connected to ") + DiscoveryManager::address());

    if (y() != 0) {
        if (m_animation->state() == QAbstractAnimation::Running) {
            m_animation->stop();
            m_animation->setStartValue(QPoint(0, y()));
        } else
            m_animation->setStartValue(QPoint(0, -height()));
        m_animation->setEndValue(QPoint(0, 0));
        m_animation->start();
    }
}

void ConnectionStatusWidget::onDisconnect()
{
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, "#C23B34");
    setPalette(palette);

    m_label->setText(tr("Disconnected from ") + DiscoveryManager::address());

    static QTimer timer;
    static auto connection = connect(&timer, &QTimer::timeout, this, [=] {
        timer.stop();
        if (!DiscoveryManager::isConnected()) {
            m_animation->setStartValue(QPoint(0, 0));
            m_animation->setEndValue(QPoint(0, -height()));
            m_animation->start();
        }
    });
    Q_UNUSED(connection)
    timer.start(5000);
}
