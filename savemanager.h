#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QObject>

class SaveManagerPrivate;
class QQuickWidget;

class SaveManager : public QObject
{
		Q_OBJECT
		Q_DISABLE_COPY(SaveManager)

	public:
		struct BindingInf {
				QString sourceId;
				QString sourceProperty;
				QString targetId;
				QString targetProperty;
				QString bindingName;
		};

		explicit SaveManager(QObject *parent = 0);
        ~SaveManager();
        static void init(QQuickWidget* w);
        static SaveManager* instance();

		static bool loadDatabase();

		static bool exists(const QString& id);
		static QStringList saves();
		static QString saveDirectory(const QString& id);
		static QJsonObject getBindingSaves();
		static QJsonObject getParentalRelationships();
		static QJsonArray getPageOrders();
		static QString parentalRelationship(const QString& id);

	private:
		static SaveManagerPrivate* m_d;
};

#endif // SAVEMANAGER_H
