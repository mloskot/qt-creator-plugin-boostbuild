#ifndef BBPROJECT_HPP_INCLUDED
#define BBPROJECT_HPP_INCLUDED

// Qt Creator
#include <coreplugin/idocument.h>
#include <projectexplorer/project.h>
// Qt
#include <QString>
#include <QFutureInterface>

namespace BoostBuildProjectManager {
namespace Internal {

class ProjectFile;
class ProjectFilesFile;
class ProjectManager;
class ProjectNode;

// Represents a project node in the project explorer.
class Project : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    Project(ProjectManager* manager, QString const& fileName);
    ~Project();

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

    void refresh();
    QString filesFileName() const;
    QStringList files() const;

protected:

    // Deserializes all project data from the map object
    // Calls the base ProjectExplorer::Project::toMap function first.
    bool fromMap(QVariantMap const& map);

private:
    void parseProject();

    // Corresponding project manager passed to the constructor
    ProjectManager* manager_;

    // Jamfile passed to the constructor (Jamroot, Jamfile, Jamfile.v2).
    QString fileName_;

    // Directory name of the Jamfile.
    // Boost.Build treats each Jamfile is a separate project,
    // where hierarchy of Jamfiles makes hierarchy of projects.
    QString projectName_;

    // Auxiliary file Jamfile.${JAMFILE_FILES_EXT} with list of source files.
    // Role of this file is similar to the .files file in the GenericProjectManager,
    // hence managing of this file is implemented in very similar manner.
    QString filesFileName_;
    QStringList files_;
    QStringList filesRaw_;
    QHash<QString, QString> entriesRaw_;

    ProjectFile* projectFile_;
    ProjectNode* projectNode_;

    // TODO:
    // Add watcher for Jamfile changes.
    // Add Jamfile parsing.
};

// Manages a project files stored in .files file.
class ProjectFilesFile : public Core::IDocument
{
    Q_OBJECT

public:
    ProjectFilesFile(Project* parent, QString const& fileName);

    bool save(QString* errorString, QString const& fileName, bool autoSave);

    QString defaultPath() const;
    QString suggestedFileName() const;
    QString mimeType() const;

    bool isModified() const;
    bool isSaveAsAllowed() const;

    ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const;
    bool reload(QString* errorString, ReloadFlag flag, ChangeType type);

private:
    Project* project_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECT_HPP_INCLUDED
