#include <applicationwindow.h>
#include <view.h>
#include <centralwidget.h>
#include <connectionstatuswidget.h>
#include <discoverymanager.h>
#include <QCoreApplication>

ApplicationWindow::ApplicationWindow(QWidget* parent) : QMainWindow(parent)
  , m_view(new View(this))
  , m_connectionStatusWidget(new ConnectionStatusWidget(m_view))
  , m_centralWidget(new CentralWidget(m_view))
{
    setWindowTitle(tr("Objectwheel Interpreter"));
    setCentralWidget(m_view);
    m_view->add(Central, m_centralWidget);
    m_view->show(Central);

    // TODO: Fix, on iOS title bar and m_connectionStatusWidget are overlapping
    connect(m_centralWidget, &CentralWidget::disableDiscoveryButtonClicked,
            DiscoveryManager::instance(), &DiscoveryManager::setDisabled);
    connect(m_centralWidget, &CentralWidget::quitButtonClicked,
            QCoreApplication::instance(), &QCoreApplication::quit);
}

QSize ApplicationWindow::sizeHint() const
{
    return {342, 608};
}
