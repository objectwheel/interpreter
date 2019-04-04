#ifndef WINDOWOPERATIONS_H
#define WINDOWOPERATIONS_H

class QWindow;

namespace Ios {
namespace WindowOperations {

void lightenStatusBar();
void makeTranslucentWindow(QWindow* window);

} // WindowOperations
} // Ios

#endif // WINDOWOPERATIONS_H