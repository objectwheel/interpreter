#include <windowoperations.h>

#include <UIKit/UIView.h>
#include <UIKit/UIApplication.h>

#include <QWindow>
#include <qpa/qplatformwindow.h>

namespace Ios {
namespace WindowOperations {

void lightenStatusBar()
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
#pragma clang diagnostic pop
}

void makeTranslucentWindow(QWindow* window)
{
    auto view = (UIView*) window->handle()->winId();
    view.opaque = false;
    view.layer.opaque = false;
    view.backgroundColor = [UIColor clearColor];
    view.layer.backgroundColor = (CGColorRef) [UIColor clearColor];
    [view setNeedsDisplay];
    [view.layer setNeedsDisplay];
}

} // WindowOperations
} // Ios
