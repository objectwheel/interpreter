#include <quitbutton.h>
#include <QPainter>
#include <QBackingStore>
#include <QResizeEvent>
#include <QSvgRenderer>

QuitButton::QuitButton(QWindow* parent) : QWindow(parent)
  , m_backingStore(new QBackingStore(this))
  , m_svgRenderer(new QSvgRenderer(QString(":/images/quitbutton.svg"), this))
  , m_pressed(false)
{
    setParent(parent);
    resize({32, 32});
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
    m_pressX = event->x();
    m_pressY = event->y();
    update();
    QWindow::mousePressEvent(event);
}

void QuitButton::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pressed)
        setPosition(event->globalX() - m_pressX, event->globalY() - m_pressY);
    QWindow::mouseMoveEvent(event);
}

void QuitButton::mouseReleaseEvent(QMouseEvent* event)
{
    m_pressed = false;
    update();
    QWindow::mouseReleaseEvent(event);
}

void QuitButton::paintEvent(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    if (m_pressed)
        painter->setOpacity(1);
    else
        painter->setOpacity(0.5);
    int length = qMin(width(), height());
    m_svgRenderer->render(painter, QRectF(width() / 2.0 - length / 2.0, height() / 2.0 - length / 2.0, length, length));
}
