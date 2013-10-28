#include "bbprojectmanager.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbproject.hpp"
#include <QFileInfo>
#include <QString>

namespace BoostBuildProjectManager {
namespace Internal {

ProjectManager::ProjectManager()
{
}

QString ProjectManager::mimeType() const
{
    return QLatin1String(Constants::MIMETYPE_PROJECT);
}

ProjectExplorer::Project*
ProjectManager::openProject(QString const& fileName, QString* errorString)
{
    if (!QFileInfo(fileName).isFile())
    {
        if (errorString)
            *errorString = tr("Failed opening project '%1': Project is not a file")
                .arg(fileName);
        return 0;
    }

    return new Project(this, fileName);
}

void ProjectManager::registerProject(Project* project)
{
    Q_ASSERT(project);
    projects_.append(project);
}

void ProjectManager::unregisterProject(Project* project)
{
    Q_ASSERT(project);
    projects_.removeAll(project);
}

} // namespace Internal
} // namespace BoostBuildProjectManager
