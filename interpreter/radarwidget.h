#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>
#include <QVariantAnimation>
#include <QBasicTimer>

class RadarWidget : public QWidget
{
    Q_OBJECT

    enum {
        REFRESH_RATE = 33, // 30 fps
        FRAME_RADIUS = 64
    };

public:
    explicit RadarWidget(QWidget* parent = nullptr);

    QAbstractAnimation::State state() const;

    int period() const;

    qreal scaleFactor() const;
    void setScaleFactor(qreal scaleFactor);

public slots:
    void start(int period = 2000);
    void stop();
    void pause();
    void resume();

protected:    
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void stateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    bool m_hidden;
    qreal m_scaleFactor;
    QBasicTimer m_updateTimer;
    QVariantAnimation m_waveAnimation;
    QVariantAnimation m_needleAnimation;
    QAbstractAnimation::State m_stateBeforeHiding;
};

#endif // RADARWIDGET_H
