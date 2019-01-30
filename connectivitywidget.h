#ifndef CONNECTIVITYWIDGET_H
#define CONNECTIVITYWIDGET_H

#include <QWidget>
#include <QVariantAnimation>

class QSvgWidget;
class RadarWidget;
class QGraphicsOpacityEffect;
class QLabel;

class ConnectivityWidget final : public QWidget
{
    Q_OBJECT

public:
    enum State {
        Disabled,
        Connected,
        Searching,
    };

public:
    explicit ConnectivityWidget(QWidget* parent = nullptr);

    State state() const;
    void setState(State state);

private slots:
    void onAnimationValueChange(const QVariant& value);

private:
    QWidget* stateWidget(State state) const;
    QGraphicsOpacityEffect* opacityEffect(const QWidget* widget) const;

protected:
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent* event) override;

private:
    State m_state;
    State m_oldState;
    QVariantAnimation m_opacityAnimation;
    QSvgWidget* m_disabledWidget;
    QSvgWidget* m_connectedWidget;
    RadarWidget* m_searchingWidget;
    QLabel* m_statusLabel;

};

#endif // CONNECTIVITYWIDGET_H