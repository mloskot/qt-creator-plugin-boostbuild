#ifndef BBPROJECT_HPP_INCLUDED
#define BBPROJECT_HPP_INCLUDED

#include <projectexplorer/project.h>

namespace Core { class IDocument; }

namespace BoostBuildProjectManager {
namespace Internal {

class ProjectFile;
class ProjectManager;

class Project : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    Project(ProjectManager* manager, QString const& fileName);
    ~Project();

    //
    // ProjectExplorer::Project
    //
    QString displayName() const;
    Core::IDocument* document() const;
    ProjectExplorer::IProjectManager* projectManager() const;
    ProjectExplorer::ProjectNode* rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;

private:
    ProjectManager* manager_;
    ProjectFile* projectFile_;
    QString fileName_; // Jamfile.v2 path
    QString filesFileName_;
    QString projectName_;

signals:

public slots:

};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECT_HPP_INCLUDED
