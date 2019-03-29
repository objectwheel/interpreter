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
    resize({36, 36});
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
    QWindow::resizeEvent(event);
    m_backingStore->resize(event->size());
}

void QuitButton::exposeEvent(QExposeEvent* event)
{
    QWindow::exposeEvent(event);
    if (isExposed())
        repaint();
}

void QuitButton::mousePressEvent(QMouseEvent* event)
{
    QWindow::mousePressEvent(event);
    m_pressed = true;
    m_moved = false;
    m_pressX = event->globalX() - x();
    m_pressY = event->globalY() - y();
    repaint();
}

void QuitButton::mouseMoveEvent(QMouseEvent* event)
{
    QWindow::mouseMoveEvent(event);
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

}

void QuitButton::mouseReleaseEvent(QMouseEvent* event)
{
    QWindow::mouseReleaseEvent(event);
    m_pressed = false;
    repaint();
    if (!m_moved)
        emit clicked();
}

void QuitButton::paintEvent(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    qreal length = qMin(width(), height()) - 0.5;
    m_svgRenderer->render(painter, {(width() - length) / 2, (height() - length) / 2, length, length});
}
