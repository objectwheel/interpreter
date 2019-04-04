#include <quitbutton.h>

#include <QSvgRenderer>
#include <QBackingStore>
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>

#if defined(Q_OS_IOS)
#include <windowoperations.h>
#include <qpa/qplatformbackingstore.h>
#include <private/qhighdpiscaling_p.h>
#endif

QuitButton::QuitButton(QWindow* parent) : QWindow(parent)
  , m_x(0)
  , m_y(0)
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
#if defined(Q_OS_IOS)
    create();
    Ios::WindowOperations::makeTranslucentWindow(this);
#endif
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
    painter.setRenderHint(QPainter::Antialiasing);
    paintEvent(&painter);
    painter.end();
    m_backingStore->endPaint();

#if defined(Q_OS_IOS)
    static QPlatformTextureList emptyTextureList;
    m_backingStore->handle()->composeAndFlush(this, QHighDpi::toNativeLocalRegion(rect, this),
                    QHighDpi::toNativeLocalPosition(QPoint(), this), &emptyTextureList, true);
#else
    m_backingStore->flush(rect);
#endif
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
    repaint();
}

void QuitButton::mousePressEvent(QMouseEvent* event)
{
    QWindow::mousePressEvent(event);
    m_x = event->globalX() - x();
    m_y = event->globalY() - y();
    m_moved = false;
    m_pressed = true;
    repaint();
}

void QuitButton::mouseReleaseEvent(QMouseEvent* event)
{
    QWindow::mouseReleaseEvent(event);
    m_pressed = false;
    repaint();
    if (!m_moved)
        emit clicked();
}

void QuitButton::mouseMoveEvent(QMouseEvent* event)
{
    QWindow::mouseMoveEvent(event);
    if (m_pressed) {
        QPoint startPos(m_x, m_y);
        QPoint currentPos(event->globalX(), event->globalY());
        if ((startPos - currentPos).manhattanLength() >=
                QApplication::startDragDistance()) {
            QPoint nextPos = currentPos - startPos;
            if (nextPos.x() < 0)
                nextPos.setX(0);
            if (nextPos.y() < 0)
                nextPos.setY(0);
            if (nextPos.x() > screen()->availableGeometry().width() - width())
                nextPos.setX(screen()->availableGeometry().width() - width());
            if (nextPos.y() > screen()->availableGeometry().height() - height())
                nextPos.setY(screen()->availableGeometry().height() - height());
            m_moved = true;
            setPosition(nextPos);
            repaint();
        }
    }
}

void QuitButton::paintEvent(QPainter* painter)
{
    qreal length = qMin(width(), height()) - 0.5;
    m_svgRenderer->render(painter, {(width() - length) / 2, (height() - length) / 2, length, length});

    if (m_pressed) {
        painter->setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter->fillRect(QRect(0, 0, width(), height()), "#30000000");
    }
}
