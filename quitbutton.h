#ifndef QUITBUTTON_H
#define QUITBUTTON_H

#include <QWindow>

class QSvgRenderer;

class QuitButton final : public QWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(QuitButton)

public:
    explicit QuitButton(QWindow* parent = nullptr);

public slots:
    void update();
    void repaint();

private:
    bool event(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void exposeEvent(QExposeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPainter* painter);

private:
    QBackingStore* m_backingStore;
    QSvgRenderer* m_svgRenderer;
    bool m_pressed;
    int m_pressX;
    int m_pressY;
};

#endif // QUITBUTTON_H