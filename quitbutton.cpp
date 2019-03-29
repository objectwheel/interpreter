#include <quitbutton.h>
#include <utilityfunctions.h>

#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QWindow>

QuitButton::QuitButton(QWidget* parent) : QAbstractButton(parent)
  , m_x(0)
  , m_y(0)
  , m_moved(false)
  , m_svgRenderer(QString(":/images/quitbutton.svg"))
{
    resize({42, 42});
    setAutoFillBackground(false);
    setWindowFlags(Qt::Tool
             | Qt::FramelessWindowHint
             | Qt::WindowDoesNotAcceptFocus
             | Qt::WindowStaysOnTopHint
             | Qt::BypassWindowManagerHint
             | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void QuitButton::mousePressEvent(QMouseEvent* event)
{
    QAbstractButton::mousePressEvent(event);
    m_x = event->globalX() - x();
    m_y = event->globalY() - y();
    m_moved = false;
    repaint();
}

void QuitButton::mouseMoveEvent(QMouseEvent* event)
{
    QAbstractButton::mouseMoveEvent(event);
    QWindow* window = UtilityFunctions::window(this);
    QPoint startPos(m_x, m_y);
    QPoint currentPos(event->globalX(), event->globalY());
    if (window && (startPos - currentPos).manhattanLength() >=
            QApplication::startDragDistance()) {
        QPoint nextPos = currentPos - startPos;
        if (nextPos.x() < 0)
            nextPos.setX(0);
        if (nextPos.y() < 0)
            nextPos.setY(0);
        if (nextPos.x() > window->screen()->availableGeometry().width() - width())
            nextPos.setX(window->screen()->availableGeometry().width() - width());
        if (nextPos.y() > window->screen()->availableGeometry().height() - height())
            nextPos.setY(window->screen()->availableGeometry().height() - height());
        m_moved = true;
        move(nextPos);
        repaint();
    }
}

void QuitButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    qreal length = qMin(width(), height()) - 0.5;
    m_svgRenderer.render(&painter, {(width() - length) / 2, (height() - length) / 2, length, length});

    if (isDown()) {
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.fillRect(rect(), "#30000000");
    }
}
