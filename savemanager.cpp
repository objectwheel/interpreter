#include <savemanager.h>
#include <filemanager.h>
#include <QQmlEngine>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>
#include <QUrl>
#include <fit.h>
#include <QQmlComponent>
#include <QEventLoop>
#include <QApplication>
#include <QQmlIncubator>
#include <QQuickWidget>
#include <QQmlProperty>
#include <zipper.h>

using namespace Fit;

#define DEFAULT_PROJECT_FOLDER "project"
#define SAVE_DIRECTORY "dashboard"
#define PARENTAL_RELATIONSHIP_FILE "parental_relationship.json"
#define PAGE_ORDER_FILE "page_order.json"
#define BINDINGS_FILE "bindings.json"
#define BINDING_SOURCE_ID_LABEL "sourceId"
#define BINDING_SOURCE_PROPERTY_LABEL "sourceProperty"
#define BINDING_TARGET_ID_LABEL "targetId"
#define BINDING_TARGET_PROPERTY_LABEL "targetProperty"

#define PAGE_CODE "\
import QtQuick 2.0 \n\
Item { \n\
            id:%1 \n\
                   \n\
                   function show() { \n\
                for (var i = 0; i < swipeView.count; i++) { \n\
                    if (swipeView.itemAt(i) === %1) { \n\
                        swipeView.currentIndex = i \n\
                                                    } \n\
                                                          } \n\
                                   } \n\
            \n\
     }"

class SaveManagerPrivate
{
	public:
		SaveManagerPrivate(SaveManager* uparent);
        inline QString defaultProjectDirectory() const;
		QString generateSaveDirectory(const QString& id) const;
        void changePageWithoutSave(const QString& from, QString& to);
        void addPageWithoutSave(QString& name);
		void createPages(const QJsonArray& pages);
        bool addControlWithoutSave(const QUrl& url, const QString& parent);
		bool fillDashboard(const QJsonObject& parentalRelationships, const QJsonArray& pages);
        void fillBindings(const QJsonObject& bindingSaves);

	public:
		SaveManager* parent = nullptr;
        QQmlEngine* engine;
        QQmlContext* rootContext;
        QQuickItem* swipeItem;
};

SaveManagerPrivate::SaveManagerPrivate(SaveManager* uparent)
	: parent(uparent)
{
#if defined(Q_OS_ANDROID)
    auto data = rdfile("assets:/project.zip");
    mkdir(defaultProjectDirectory());
    Zipper::extractZip(data, defaultProjectDirectory());
#endif
}

QString SaveManagerPrivate::defaultProjectDirectory() const
{
    QString baseDir;
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID) || defined(Q_OS_WINPHONE)
    baseDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0);
#else
    baseDir = QCoreApplication::applicationDirPath();
#endif
    return baseDir + separator() + DEFAULT_PROJECT_FOLDER;
}

inline QString SaveManagerPrivate::generateSaveDirectory(const QString& id) const
{
    auto projectDir = defaultProjectDirectory();
    return projectDir + separator() + SAVE_DIRECTORY + separator() + id;
}

void SaveManagerPrivate::createPages(const QJsonArray& pages)
{
    auto firstPage = pages[0].toString();
    changePageWithoutSave("page1", firstPage);
	for (int i = 1; i < pages.size(); i++) {
		auto currPage = pages[i].toString();
        addPageWithoutSave(currPage);
	}
}

void SaveManagerPrivate::addPageWithoutSave(QString& name)
{
    QQmlComponent c(qmlEngine((QObject*)swipeItem));
    c.setData(QByteArray().insert(0,QString(PAGE_CODE).arg(name)), QUrl());
    auto item = qobject_cast<QQuickItem*>(c.create(qmlContext((QObject*)swipeItem)));
    if (!item) qFatal("SaveManagerPrivate::addPageWithoutSave : Error occurred");
    item->setParentItem(swipeItem);
    rootContext->setContextProperty(name, item);
}

void SaveManagerPrivate::changePageWithoutSave(const QString& from, QString& to)
{
    auto v = rootContext->contextProperty(from);
    auto selectedItem = qobject_cast<QQuickItem*>(v.value<QObject*>());
    if (!selectedItem) qFatal("SaveManagerPrivate::changePageWithoutSave : Error occurred");
    rootContext->setContextProperty(from, 0);
    rootContext->setContextProperty(to, selectedItem);
}

bool SaveManagerPrivate::addControlWithoutSave(const QUrl& url, const QString& parent)
{
    engine->clearComponentCache();
    QQmlComponent component(engine);
    component.loadUrl(url);

    QQmlIncubator incubator;
    component.create(incubator, rootContext);
    while (incubator.isLoading()) {
        QApplication::processEvents(QEventLoop::AllEvents, 50);
    }
    QQuickItem *qml = qobject_cast<QQuickItem*>(incubator.object());
    if (component.isError() || !qml) return false;

    QString componentName = fname(dname(url.toLocalFile()));
    auto parentProperty = rootContext->contextProperty(parent);
    auto parentItem = qobject_cast<QQuickItem*>(parentProperty.value<QObject*>());
    if (!parentItem) qFatal("SaveManagerPrivate::addControlWithoutSave : Error occurred");
    rootContext->setContextProperty(componentName, qml);
    qml->setParentItem(parentItem);
    fit(qml, Fit::WidthHeight);
    qml->setPosition(QPointF(fit(qml->x()), fit(qml->y())));
    //FIXME: QTimer::singleShot(200, [qml] { m_d->parent->fixWebViewPosition(qml); });
    return true;
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
                if (!addControlWithoutSave(QUrl::fromLocalFile(saveId + separator() + "main.qml"), parentalRelationships[key].toString()))
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
//        BindingWidget::addBindingWithoutSave(inf);
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

void SaveManager::init(QQuickWidget* w)
{
    new SaveManager;
    m_d->rootContext = w->rootContext();
    m_d->engine = w->engine();
    auto v = QQmlProperty::read(w->rootObject(), "swipeView", w->rootContext());
    auto view = qobject_cast<QQuickItem*>(v.value<QObject*>());
    if (!view) qFatal("SaveManager::init : Error occurred");
    m_d->swipeItem = view;
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
    auto projDir = m_d->defaultProjectDirectory();
	auto bindingsFile = projDir + separator() + SAVE_DIRECTORY + separator() + BINDINGS_FILE;
	return QJsonDocument::fromJson(rdfile(bindingsFile)).object();
}

QJsonObject SaveManager::getParentalRelationships()
{
    auto projDir = m_d->defaultProjectDirectory();
	auto parentalFile = projDir + separator() + SAVE_DIRECTORY + separator() + PARENTAL_RELATIONSHIP_FILE;
	return QJsonDocument::fromJson(rdfile(parentalFile)).object();
}

QJsonArray SaveManager::getPageOrders()
{
    auto projDir = m_d->defaultProjectDirectory();
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
    auto projectDir = m_d->defaultProjectDirectory();
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

