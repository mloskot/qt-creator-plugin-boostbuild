#ifndef BBPROJECT_HPP_INCLUDED
#define BBPROJECT_HPP_INCLUDED

#include <projectexplorer/project.h>

namespace Core { class IDocument; }

namespace BoostBuildProjectManager {
namespace Internal {

class ProjectFile;
class ProjectManager;
class ProjectNode;

//  Implements a project node in the project explorer.
class Project : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    Project(ProjectManager* manager, QString const& fileName);
    ~Project();

    //
    // ProjectExplorer::Project overridables
    //
    QString displayName() const;
    Core::Id id() const;
    Core::IDocument* document() const;
    ProjectExplorer::IProjectManager* projectManager() const;
    ProjectExplorer::ProjectNode* rootProjectNode() const;
    QStringList files(FilesMode fileMode) const;

    // Indicates if "Configure Project" panel should be opened on project loading.
    // This step may be used to select a kit, build directory, and others.
    // See QmakeProjectManager as an example.
    bool needsConfiguration() const;

protected:

    // Deserializes all project data from the map object
    // Calls the base ProjectExplorer::Project::toMap function first.
    bool fromMap(QVariantMap const& map);

private:
    // Corresponding project manager passed to the constructor
    ProjectManager* manager_;

    // Jamfile passed to the constructor (Jamroot, Jamfile, Jamfile.v2).
    QString fileName_;

    // Directory name of the Jamfile.
    // Boost.Build treats each Jamfile is a separate project,
    // where hierarchy of Jamfiles makes hierarchy of projects.
    //
    // TODO: Parse jamfile looking for project rule,
    QString projectName_;

    // Auxiliary file Jamfile.v2.files with list of sources and headers.
    // Role of this file is similar to the .files file in the Generic Project.
    QString filesFileName_;
    QStringList files_;

    ProjectFile* projectFile_;
    ProjectNode* projectNode_;

    // TODO:
    // Add watche for Jamfile changes.
    // Add Jamfile parsing.
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECT_HPP_INCLUDED
