#ifndef BBPROJECTMANAGER_HPP_INCLUDED
#define BBPROJECTMANAGER_HPP_INCLUDED

#include <projectexplorer/iprojectmanager.h>
#include <QList>
#include <QString>

namespace ProjectExplorer {
class Project;
}

namespace BoostBuildProjectManager {
namespace Internal {

class Project;

class ProjectManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    ProjectManager();

    void registerProject(Project* project);
    void unregisterProject(Project* project);

    //
    // ProjectExplorer::IProjectManager
    //
    QString mimeType() const;
    ProjectExplorer::Project* openProject(QString const& fileName, QString* errorString);
    private:
    QList<Project*> projects_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECTMANAGER_HPP_INCLUDED
