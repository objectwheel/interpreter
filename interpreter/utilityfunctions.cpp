#include <utilityfunctions.h>
#include <QOperatingSystemVersion>
#include <QAbstractButton>

namespace UtilityFunctions {

namespace Internal {
void pushHelper(QDataStream&) {}
void pullHelper(QDataStream&) {}
} // Internal

QWindow* window(const QWidget* widget)
{
    Q_ASSERT(widget);
    QWindow* winHandle = widget->windowHandle();
    if (!winHandle) {
        if (const QWidget* nativeParent = widget->nativeParentWidget())
            winHandle = nativeParent->windowHandle();
    }
    return winHandle;
}

QFont systemDefaultFont()
{
    QFont font;
#if defined(Q_OS_MACOS)
    if (QOperatingSystemVersion::current() >= QOperatingSystemVersion::MacOSCatalina)
        font.setFamily("SF UI Display");
    else
        font.setFamily(".SF NS Display");
#elif defined(Q_OS_WIN)
    font.setFamily("Segoe UI");
#endif
    font.setPixelSize(13);
    return font;
}

QMessageBox::StandardButton showMessage(QWidget* parent, const QString& title, const QString& text,
                                        QMessageBox::Icon icon, QMessageBox::StandardButtons buttons,
                                        QMessageBox::StandardButton defaultButton, bool modal)
{
    QMessageBox dialog(parent);
    dialog.setIcon(icon);
    dialog.setModal(modal);
    dialog.setStandardButtons(buttons);
    dialog.setDefaultButton(defaultButton);
#if !defined(Q_OS_MACOS)
    dialog.QWidget::setWindowTitle(title);
    dialog.setText(text);
#else
    dialog.setText(title);
    dialog.setInformativeText(text);
    for (QAbstractButton* button : dialog.buttons())
        button->setCursor(Qt::PointingHandCursor);
    if (auto label = dialog.findChild<QWidget*>(QStringLiteral("qt_msgbox_label"))) {
        int MIN_WIDTH = qMax(label->fontMetrics().horizontalAdvance(title), 300);
        label->setStyleSheet(QStringLiteral("QLabel { min-width: %1px; }").arg(MIN_WIDTH));
    }
#endif
    return static_cast<QMessageBox::StandardButton>(dialog.exec());
}

} // UtilityFunctions