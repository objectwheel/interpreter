#ifndef QMLCOMPONENT_H
#define QMLCOMPONENT_H

#include <QQmlComponent>

class QmlComponentPrivate;

class QmlComponent final : public QQmlComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QmlComponent)
    Q_DISABLE_COPY(QmlComponent)

public:
    using QQmlComponent::QQmlComponent;
    QObject* beginCreate(QQmlContext* publicContext) override;
    bool isCompletePending() const;
};

#endif // QMLCOMPONENT_H