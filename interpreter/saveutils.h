#ifndef SAVEUTILS_H
#define SAVEUTILS_H

#include <QVariant>

namespace SaveUtils {

enum ControlProperties : quint32 { // ### Application related properties
    ControlSignature = 0x1000,
    ControlVersion,
    ControlId,
    ControlUid,
    ControlIndex
};

enum DesignProperties : quint32 { // ### Designer related properties
    DesignSignature = 0x4000,
    DesignVersion,
    DesignPosition,
    DesignSize
};

enum ProjectProperties : quint32 { // ### Designer related properties
    ProjectSignature = 0x2000,
    ProjectVersion,
    ProjectUid,
    ProjectName,
    ProjectDescription,
    ProjectCreationDate,
    ProjectModificationDate,
    ProjectSize,
    ProjectTheme,      // ### TODO: Marked for removal, qtquickcontrols2.conf takes place
    ProjectHdpiScaling // ### TODO: Marked for removal, move it to qtquickcontrols2.conf, it is a QSettings ini, right? then we should be able to add custom values right?
};

enum UserProperties : quint32 { // ### Designer related properties
    UserSignature = 0x3000,
    UserVersion,
    UserEmail,
    UserPassword
};

QString controlMainQmlFileName();
QString toControlThisDir(const QString& controlDir);
QString toControlChildrenDir(const QString& controlDir);
QString toProjectDesignsDir(const QString& projectDir);
QString toProjectImportsDir(const QString& projectDir);
QString toProjectAssetsDir(const QString& projectDir);
QString toUserProjectsDir(const QString& userDir);
QString toControlMainQmlFile(const QString& controlDir);
QString toDoubleUp(const QString& path);

bool isForm(const QString& controlDir);
bool isControlValid(const QString& controlDir);
bool isDesignValid(const QString& controlDir);
bool isProjectValid(const QString& projectDir);
bool isUserValid(const QString& userDir);

quint32 controlIndex(const QString& controlDir);
QString controlId(const QString& controlDir);
QString controlUid(const QString& controlDir);
QPointF designPosition(const QString& controlDir);
QSizeF designSize(const QString& controlDir);

bool projectHdpiScaling(const QString& projectDir);
qint64 projectSize(const QString& projectDir);
QString projectUid(const QString& projectDir);
QString projectName(const QString& projectDir);
QString projectDescription(const QString& projectDir);
QDateTime projectCreationDate(const QString& projectDir);
QDateTime projectModificationDate(const QString& projectDir);
QByteArray projectTheme(const QString& projectDir);

QString userEmail(const QString& userDir);
QByteArray userPassword(const QString& userDir);

bool setProperty(const QString& controlDir, ControlProperties property, const QVariant& value);
bool setProperty(const QString& controlDir, DesignProperties property, const QVariant& value);
bool setProperty(const QString& projectDir, ProjectProperties property, const QVariant& value);
bool setProperty(const QString& userDir, UserProperties property, const QVariant& value);

QVector<QString> formPaths(const QString& projectDir);
QVector<QString> childrenPaths(const QString& controlDir, bool recursive = true);

} // SaveUtils

#endif // SAVEUTILS_H
