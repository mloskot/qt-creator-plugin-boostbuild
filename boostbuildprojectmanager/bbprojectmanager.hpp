#ifndef BBPROJECTMANAGER_HPP
#define BBPROJECTMANAGER_HPP

// Qt Creator
#include <projectexplorer/iprojectmanager.h>
// Qt
#include <QList>
#include <QString>

namespace ProjectExplorer {
class Project;
}

namespace BoostBuildProjectManager {
namespace Internal {

class Project;

// Sole implementation of the IProjectManager class for the extension.
class ProjectManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    ProjectManager();

    QString mimeType() const;

    // Creates new instance of Project class.
    ProjectExplorer::Project*
    openProject(QString const& fileName, QString* errorString);

    void registerProject(Project* project);
    void unregisterProject(Project* project);

private:
    QList<Project*> projects_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECTMANAGER_HPP
