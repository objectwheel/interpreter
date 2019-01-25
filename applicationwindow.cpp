#include <applicationwindow.h>
#include <view.h>
#include <centralwidget.h>

ApplicationWindow::ApplicationWindow(QWidget* parent) : QMainWindow(parent)
  , m_view(new View(this))
  , m_centralWidget(new CentralWidget(this))
{
    setCentralWidget(m_view);
    m_view->add(Central, m_centralWidget);
    m_view->show(Central);
}

QSize ApplicationWindow::sizeHint() const
{
    return {342, 608};
}
