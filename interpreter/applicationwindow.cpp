#include <applicationwindow.h>
#include <view.h>
#include <centralwidget.h>
#include <connectionstatuswidget.h>
#include <broadcastingmanager.h>
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
    setWindowTitle(QStringLiteral(APP_NAME));
    setCentralWidget(m_view);

#if defined(Q_OS_IOS)
    Ios::WindowOperations::lightenStatusBar();
#endif

    m_view->add(Central, m_centralWidget);
    m_view->show(Central);

    connect(m_centralWidget, &CentralWidget::stopBroadcastingButtonClicked,
            BroadcastingManager::instance(), &BroadcastingManager::setDisabled);
    connect(m_centralWidget, &CentralWidget::quitButtonClicked,
            QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);

    resize(sizeHint());
}

CentralWidget* ApplicationWindow::centralWidget() const
{
    return m_centralWidget;
}

QSize ApplicationWindow::sizeHint() const
{
    return {342, 686};
}

bool ApplicationWindow::mightThemeChange(const QString& uid)
{
    if (QuickTheme::activeThemeDiffersFrom(ProjectManager::projectDirectory(uid))) {
#if defined(Q_OS_IOS)
        static const QString message = tr("Theme change detected on the project. Please "
                                          "restart the application for this change to take effect.");
        static const QMessageBox::StandardButtons btns = QMessageBox::Close;
        static const QMessageBox::StandardButton defaultBtn = QMessageBox::Close;
#else
        static const QString message = tr("Theme change detected on the project. Would you like to "
                                          "restart the application for this change to take effect?");
        static const QMessageBox::StandardButtons btns = QMessageBox::Yes | QMessageBox::No;
        static const QMessageBox::StandardButton defaultBtn = QMessageBox::Yes;
#endif
        QMessageBox warning;
        warning.QWidget::setWindowTitle(tr("Theme Change"));
        warning.setIcon(QMessageBox::Question);
        warning.setText(message);
        warning.setStandardButtons(btns);
        warning.setDefaultButton(defaultBtn);
        int ret = warning.exec();
        if (ret & QMessageBox::Yes)
            return true;
        if (ret & QMessageBox::Close)
            return true;
    }
    return false;
}
