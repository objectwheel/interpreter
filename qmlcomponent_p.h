#ifndef QMLCOMPONENTPRIVATE_H
#define QMLCOMPONENTPRIVATE_H

#include <QtQml/private/qqmlcomponent_p.h>

class QmlComponent;

class QmlComponentPrivate : public QQmlComponentPrivate
{
        Q_DECLARE_PUBLIC(QmlComponent)

    public:
        QObject* beginCreate(QQmlContextData* context);
};

#endif // QMLCOMPONENTPRIVATE_H