#include <savemanager.h>
#include <filemanager.h>
#include <QQmlEngine>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#define SAVE_DIRECTORY "dashboard"
#define PARENTAL_RELATIONSHIP_FILE "parental_relationship.json"
#define PAGE_ORDER_FILE "page_order.json"
#define BINDINGS_FILE "bindings.json"
#define BINDING_SOURCE_ID_LABEL "sourceId"
#define BINDING_SOURCE_PROPERTY_LABEL "sourceProperty"
#define BINDING_TARGET_ID_LABEL "targetId"
#define BINDING_TARGET_PROPERTY_LABEL "targetProperty"

class SaveManagerPrivate
{
	public:
		SaveManagerPrivate(SaveManager* uparent);
		QString generateSaveDirectory(const QString& id) const;
		void createPages(const QJsonArray& pages);
		bool fillDashboard(const QJsonObject& parentalRelationships, const QJsonArray& pages);
        void fillBindings(const QJsonObject& bindingSaves);

	public:
		SaveManager* parent = nullptr;
};

SaveManagerPrivate::SaveManagerPrivate(SaveManager* uparent)
	: parent(uparent)
{
}

inline QString SaveManagerPrivate::generateSaveDirectory(const QString& id) const
{
	auto projectDir = ProjectManager::projectDirectory(ProjectManager::currentProject());
	if (projectDir.isEmpty()) return projectDir;
	return projectDir + separator() + SAVE_DIRECTORY + separator() + id;
}

void SaveManagerPrivate::createPages(const QJsonArray& pages)
{
	auto firstPage = pages[0].toString();
	PagesWidget::changePageWithoutSave("page1", firstPage);
	for (int i = 1; i < pages.size(); i++) {
		auto currPage = pages[i].toString();
		PagesWidget::addPageWithoutSave(currPage);
	}
}

bool SaveManagerPrivate::fillDashboard(const QJsonObject& parentalRelationships, const QJsonArray& pages)
{
	QStringList createdObjects;
	for (auto page : pages) {
		createdObjects << page.toString();
	}
	while (!createdObjects.isEmpty()) {
		for (auto key : parentalRelationships.keys()) {
			if (parentalRelationships[key].toString() == createdObjects[0]) {
				auto saveId = parent->saveDirectory(key);
				if (saveId.isEmpty()) return false;
				if (!MainWindow::addControlWithoutSave(QUrl::fromLocalFile(saveId + separator() + "main.qml"),
													   parentalRelationships[key].toString()))
					return false;
				createdObjects.append(key);
			}
		}
		createdObjects.removeFirst();
	}
	return true;
}

void SaveManagerPrivate::fillBindings(const QJsonObject& bindingSaves)
{
    SaveManager::BindingInf inf;
    for (auto bindingKey : bindingSaves.keys()) {
        inf.bindingName = bindingKey;
        inf.sourceId = bindingSaves[bindingKey].toObject()[BINDING_SOURCE_ID_LABEL].toString();
        inf.sourceProperty = bindingSaves[bindingKey].toObject()[BINDING_SOURCE_PROPERTY_LABEL].toString();
        inf.targetId = bindingSaves[bindingKey].toObject()[BINDING_TARGET_ID_LABEL].toString();
        inf.targetProperty = bindingSaves[bindingKey].toObject()[BINDING_TARGET_PROPERTY_LABEL].toString();
        BindingWidget::addBindingWithoutSave(inf);
    }
}

SaveManagerPrivate* SaveManager::m_d = nullptr;

SaveManager::SaveManager(QObject *parent)
	: QObject(parent)
{
	if (m_d) return;
    m_d = new SaveManagerPrivate(this);
}

SaveManager::~SaveManager()
{
    delete m_d;
}

SaveManager* SaveManager::instance()
{
	return m_d->parent;
}

QString SaveManager::saveDirectory(const QString& id)
{
	if (!exists(id)) return QString();
	return m_d->generateSaveDirectory(id);
}

QJsonObject SaveManager::getBindingSaves()
{
	auto projDir = ProjectManager::projectDirectory(ProjectManager::currentProject());
	if (projDir.isEmpty()) return QJsonObject();
	auto bindingsFile = projDir + separator() + SAVE_DIRECTORY + separator() + BINDINGS_FILE;
	return QJsonDocument::fromJson(rdfile(bindingsFile)).object();
}

QJsonObject SaveManager::getParentalRelationships()
{
	auto projDir = ProjectManager::projectDirectory(ProjectManager::currentProject());
	if (projDir.isEmpty()) return QJsonObject();
	auto parentalFile = projDir + separator() + SAVE_DIRECTORY + separator() + PARENTAL_RELATIONSHIP_FILE;
	return QJsonDocument::fromJson(rdfile(parentalFile)).object();
}

QJsonArray SaveManager::getPageOrders()
{
	auto projDir = ProjectManager::projectDirectory(ProjectManager::currentProject());
	if (projDir.isEmpty()) return QJsonArray();
	auto pageOrderFile = projDir + separator() + SAVE_DIRECTORY + separator() + PAGE_ORDER_FILE;
	return QJsonDocument::fromJson(rdfile(pageOrderFile)).array();
}

bool SaveManager::exists(const QString& id)
{
	auto saveDir = m_d->generateSaveDirectory(id);
	if (saveDir.isEmpty()) return false;
	return ::exists(saveDir);
}

QStringList SaveManager::saves()
{
	QStringList saveList;
	auto projectDir = ProjectManager::projectDirectory(ProjectManager::currentProject());
	if (projectDir.isEmpty()) return saveList;
	auto saveBaseDir = projectDir + separator() + SAVE_DIRECTORY;
	for (auto save : lsdir(saveBaseDir)) {
		saveList << save;
	}
    return saveList;
}

bool SaveManager::loadDatabase()
{
	QJsonArray pageOrder = getPageOrders();
	QJsonObject parentalRelationship = getParentalRelationships();
	QJsonObject bindingSaves = getBindingSaves();
	if (pageOrder.isEmpty()) return false;
	if (parentalRelationship.size() != saves().size()) return false;
	m_d->createPages(pageOrder);
	if (!m_d->fillDashboard(parentalRelationship, pageOrder)) return false;
    m_d->fillBindings(bindingSaves);
    return true;
}

QString SaveManager::parentalRelationship(const QString& id)
{
	auto jObj = getParentalRelationships();
	return jObj[id].toString();
}

