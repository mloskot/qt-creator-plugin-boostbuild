#ifndef BBPROJECTMANAGER_HPP_INCLUDED
#define BBPROJECTMANAGER_HPP_INCLUDED

#include <projectexplorer/iprojectmanager.h>

namespace BoostBuildProjectManager {
namespace Internal {

class ProjectManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    ProjectManager();

    //
    // ProjectExplorer::IProjectManager
    //
    QString mimeType() const;
    ProjectExplorer::Project* openProject(QString const& fileName, QString* errorString);

signals:

public slots:

};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECTMANAGER_HPP_INCLUDED
