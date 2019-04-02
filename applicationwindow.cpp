#include <applicationwindow.h>
#include <view.h>
#include <centralwidget.h>
#include <connectionstatuswidget.h>
#include <discoverymanager.h>
#include <projectmanager.h>
#include <quicktheme.h>

#include <QApplication>
#include <QMessageBox>

#if defined(Q_OS_IOS)
#include <windowoperations.h>
#endif

ApplicationWindow::ApplicationWindow(QWidget* parent) : QMainWindow(parent)
  , m_view(new View(this))
  , m_connectionStatusWidget(new ConnectionStatusWidget(m_view))
  , m_centralWidget(new CentralWidget(m_view))
{
    setWindowTitle(tr("Objectwheel Interpreter"));
    setCentralWidget(m_view);

#if defined(Q_OS_IOS)
    Ios::WindowOperations::lightenStatusBar();
#endif

    m_view->add(Central, m_centralWidget);
    m_view->show(Central);

    // TODO: Fix, on iOS title bar and m_connectionStatusWidget are overlapping
    connect(m_centralWidget, &CentralWidget::disableDiscoveryButtonClicked,
            DiscoveryManager::instance(), &DiscoveryManager::setDisabled);
    connect(m_centralWidget, &CentralWidget::quitButtonClicked,
            QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);
}

CentralWidget* ApplicationWindow::centralWidget() const
{
    return m_centralWidget;
}

QSize ApplicationWindow::sizeHint() const
{
    return {342, 608};
}

bool ApplicationWindow::mightThemeChange(const QString& uid)
{
    if (QuickTheme::activeThemeDiffersFrom(ProjectManager::projectDirectory(uid))) {
#if defined(Q_OS_IOS)
        static const QString message = tr("Theme change detected on the project. Please "
                                          "restart the application for this change to take effect.");
        static const QMessageBox::StandardButtons btn = QMessageBox::Ok;
#else
        static const QString message = tr("Theme change detected on the project. Would you like to "
                                          "restart the application for this change to take effect?");
        static const QMessageBox::StandardButtons btn = QMessageBox::Yes | QMessageBox::No;
#endif
        int ret = QMessageBox::warning(nullptr, tr("Theme Change"), message, btn);
        if (ret & QMessageBox::Yes)
            return true;
    }
    return false;
}
