#include <projectmanager.h>

ProjectManager* ProjectManager::s_instance = nullptr;

ProjectManager::ProjectManager(QObject* parent) : QObject(parent)
{
    s_instance = this;
}

ProjectManager::~ProjectManager()
{
    s_instance = nullptr;
}

ProjectManager* ProjectManager::instance()
{
    return s_instance;
}

QString ProjectManager::projectPath(const QString& uid)
{

}
