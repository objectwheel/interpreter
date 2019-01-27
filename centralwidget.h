#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QLabel;
class QVBoxLayout;
class RadarWidget;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget* parent = nullptr);

private:
    QVBoxLayout* m_layout;
    QLabel* m_searchingLabel;
};

#endif // CENTRALWIDGET_H