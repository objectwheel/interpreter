#ifndef QMLCOMPONENT_H
#define QMLCOMPONENT_H

#include <QtQml/private/qqmlcomponent_p.h>

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

class QmlComponentPrivate : public QQmlComponentPrivate
{
    Q_DECLARE_PUBLIC(QmlComponent)

public:
    QObject* beginCreate(QQmlContextData* context);
};

#endif // QMLCOMPONENT_H