#ifndef CONNECTIVITYWIDGET_H
#define CONNECTIVITYWIDGET_H

#include <QWidget>
#include <QVariantAnimation>

class QSvgWidget;
class RadarWidget;
class QGraphicsOpacityEffect;

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

protected:
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent* event) override;

private:
    State m_state;
    State m_oldState;
    QSvgWidget* m_disabledWidget;
    QSvgWidget* m_connectedWidget;
    RadarWidget* m_searchingWidget;
    QGraphicsOpacityEffect* m_opacityEffect;
    QVariantAnimation m_opacityAnimation;

};

#endif // CONNECTIVITYWIDGET_H