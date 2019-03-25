#include <quitbutton.h>
#include <QPainter>
#include <QBackingStore>
#include <QResizeEvent>
#include <QSvgRenderer>
#include <QApplication>
#include <QScreen>

QuitButton::QuitButton(QWindow* parent) : QWindow(parent)
  , m_pressX(0)
  , m_pressY(0)
  , m_moved(false)
  , m_pressed(false)
  , m_backingStore(new QBackingStore(this))
  , m_svgRenderer(new QSvgRenderer(QString(":/images/quitbutton.svg"), this))
{
    setParent(parent);
    resize({48, 48});
    setFlags(Qt::Tool
             | Qt::FramelessWindowHint
             | Qt::WindowDoesNotAcceptFocus
             | Qt::WindowStaysOnTopHint
             | Qt::BypassWindowManagerHint
             | Qt::NoDropShadowWindowHint);
}

void QuitButton::update()
{
    requestUpdate();
}

void QuitButton::repaint()
{
    if (!isExposed())
        return;

    QPainter painter;
    QRect rect(0, 0, width(), height());

    m_backingStore->beginPaint(rect);

    painter.begin(m_backingStore->paintDevice());
    paintEvent(&painter);
    painter.end();

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

bool QuitButton::event(QEvent* event)
{
    if (event->type() == QEvent::UpdateRequest) {
        repaint();
        return true;
    }
    return QWindow::event(event);
}

void QuitButton::resizeEvent(QResizeEvent* event)
{
    m_backingStore->resize(event->size());
    QWindow::resizeEvent(event);
}

void QuitButton::exposeEvent(QExposeEvent* event)
{
    if (isExposed())
        repaint();
    QWindow::exposeEvent(event);
}

void QuitButton::mousePressEvent(QMouseEvent* event)
{
    m_pressed = true;
    m_moved = false;
    m_pressX = event->globalX() - x();
    m_pressY = event->globalY() - y();
    repaint();
    QWindow::mousePressEvent(event);
}

void QuitButton::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pressed) {
        QPoint startPos(m_pressX, m_pressY);
        QPoint currentPos(event->globalX(), event->globalY());
        if ((startPos - currentPos).manhattanLength() >=
                QApplication::startDragDistance()) {
            m_moved = true;
            QPoint nextPos = currentPos - startPos;
            if (nextPos.x() < 0)
                nextPos.setX(0);
            if (nextPos.y() < 0)
                nextPos.setY(0);
            if (nextPos.x() > screen()->availableGeometry().width() - width())
                nextPos.setX(screen()->availableGeometry().width() - width());
            if (nextPos.y() > screen()->availableGeometry().height() - height())
                nextPos.setY(screen()->availableGeometry().height() - height());
            setPosition(nextPos);
            repaint();
        }
    }
    QWindow::mouseMoveEvent(event);
}

void QuitButton::mouseReleaseEvent(QMouseEvent* event)
{
    m_pressed = false;
    repaint();
    if (!m_moved)
        emit clicked();
    QWindow::mouseReleaseEvent(event);
}

void QuitButton::paintEvent(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    qreal length = qMin(width(), height()) - 0.5;
    m_svgRenderer->render(painter, {(width() - length) / 2, (height() - length) / 2, length, length});
}
