#ifndef EXECUTIVEWIDGET_H
#define EXECUTIVEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QQuickWindow>
#include <QPointer>
#include <executer.h>

class ExecutiveWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit ExecutiveWidget(QWidget* parent = nullptr);
        SaveUtils::Skin skin() const;
        void setSkin(SaveUtils::Skin skin);
        void setWindow(QQuickWindow* window);

    public slots:
        void stop();

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void closeEvent(QCloseEvent* event) override;

    signals:
        void done() const;

    private:
        int _x, _y;
        bool _pressed;
        SaveUtils::Skin _skin;
        QVBoxLayout* _layout;
        QQuickWindow* _window;
        QWidget* _containerWidget;
        QPointer<QQuickItem> _contentItem;
};

#endif // EXECUTIVEWIDGET_H
