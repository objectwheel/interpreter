#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>
#include <QVariantAnimation>
#include <QBasicTimer>

class RadarWidget : public QWidget
{
    Q_OBJECT

    enum {
        REFRESH_RATE = 33 // 30 fps
    };

public:
    explicit RadarWidget(QWidget* parent = nullptr);

    QAbstractAnimation::State state() const;

    int period() const;

public slots:
    void start(int period = 2000);
    void stop();
    void pause();
    void resume();

protected:
    QSize sizeHint() const override;
    void timerEvent(QTimerEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

signals:
    void stateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    QBasicTimer m_updateTimer;
    QVariantAnimation m_waveAnimation;
    QVariantAnimation m_needleAnimation;
};

#endif // RADARWIDGET_H