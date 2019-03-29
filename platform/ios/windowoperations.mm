#include <windowoperations.h>
#include <UIKit/UIApplication.h>

namespace Ios {

namespace WindowOperations {

void lightenStatusBar()
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
#pragma clang diagnostic pop
}

} // WindowOperations
} // Ios
