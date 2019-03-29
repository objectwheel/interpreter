#ifndef QUITBUTTON_H
#define QUITBUTTON_H

#include <QAbstractButton>
#include <QSvgRenderer>

class QSvgRenderer;

class QuitButton final : public QAbstractButton
{
    Q_OBJECT
    Q_DISABLE_COPY(QuitButton)

public:
    explicit QuitButton(QWidget* parent = nullptr);
    bool isMoved() const { return m_moved; }

private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    int m_x;
    int m_y;
    bool m_moved;
    QSvgRenderer m_svgRenderer;
};

#endif // QUITBUTTON_H