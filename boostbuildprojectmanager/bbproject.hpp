#ifndef BBPROJECT_HPP_INCLUDED
#define BBPROJECT_HPP_INCLUDED

#include <projectexplorer/project.h>

namespace Core { class IDocument; }

namespace BoostBuildProjectManager {
namespace Internal {

class ProjectManager;

class Project : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    Project(ProjectManager* manager, QString const& fileName);

    //
    // ProjectExplorer::Project
    //
    QString displayName() const;
    Core::IDocument* document() const;
    ProjectExplorer::IProjectManager* projectManager() const;
    ProjectExplorer::ProjectNode *rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;

private:
    ProjectManager* manager_;
    QString fileName_;
    QString projectName_;
    QString jamFileName_;
    QString filesFileName_;

signals:

public slots:

};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECT_HPP_INCLUDED
