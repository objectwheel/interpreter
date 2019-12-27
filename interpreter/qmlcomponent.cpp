#include <qmlcomponent.h>
#include <qmlcomponent_p.h>

QObject* QmlComponentPrivate::beginCreate(QQmlContextData* context)
{
    Q_Q(QQmlComponent);

    if (!context) {
        qWarning("QQmlComponent: Cannot create a component in a null context");
        return 0;
    }

    if (!context->isValid()) {
        qWarning("QQmlComponent: Cannot create a component in an invalid context");
        return 0;
    }

    if (context->engine != engine) {
        qWarning("QQmlComponent: Must create component in context from the same QQmlEngine");
        return 0;
    }

    if (state.completePending) {
        qWarning("QQmlComponent: Cannot create new component instance before completing the previous");
        return 0;
    }

    if (!q->isReady()) {
        qWarning("QQmlComponent: Component is not ready");
        return 0;
    }

    QQmlEnginePrivate *enginePriv = QQmlEnginePrivate::get(engine);

    enginePriv->inProgressCreations++;
    state.errors.clear();
    state.completePending = true;

    enginePriv->referenceScarceResources();
    QObject *rv = 0;
    state.creator.reset(new QQmlObjectCreator(context, compilationUnit, creationContext));
    rv = state.creator->create(start);
    if (!rv)
        state.errors = state.creator->errors;
    enginePriv->dereferenceScarceResources();

    if (rv) {
        QQmlData *ddata = QQmlData::get(rv);
        Q_ASSERT(ddata);
        ddata->indestructible = true;
        ddata->explicitIndestructibleSet = true;
        ddata->rootObjectInCreation = false;
    }

    return rv;
}

QObject* QmlComponent::beginCreate(QQmlContext* publicContext)
{
    Q_D(QmlComponent);

    Q_ASSERT(publicContext);
    QQmlContextData *context = QQmlContextData::get(publicContext);

    return d->beginCreate(context);
}
