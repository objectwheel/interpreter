#ifndef QMLCOMPONENT_H
#define QMLCOMPONENT_H

#include <QQmlComponent>

class QmlComponentPrivate;

class QmlComponent : public QQmlComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QmlComponent)
    Q_DISABLE_COPY(QmlComponent)

public:
    using QQmlComponent::QQmlComponent;
    QObject* beginCreate(QQmlContext* publicContext) override;
};

#endif // QMLCOMPONENT_H