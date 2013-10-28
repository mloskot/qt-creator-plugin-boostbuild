#ifndef BBPROJECTNODE_HPP_INCLUDED
#define BBPROJECTNODE_HPP_INCLUDED

#include <projectexplorer/projectnodes.h>

namespace Core {
class IDocument;
}

namespace BoostBuildProjectManager {
namespace Internal {

class Project;

// An in-memory presentation of a Project.
// Represents a file or a folder of the project tree.
// No special operations (addFiles(), removeFiles(), renameFile(), etc.) are offered.
class ProjectNode : public ProjectExplorer::ProjectNode
{
    Q_OBJECT

public:
    ProjectNode(Project* project, Core::IDocument* projectFile);

    bool hasBuildTargets() const;
    QList<ProjectExplorer::ProjectNode::ProjectAction> supportedActions(Node* node) const;
    bool canAddSubProject(QString const& filePath) const;
    bool addSubProjects(QStringList const& filePaths);
    bool removeSubProjects(QStringList const& filePaths);
    bool addFiles(QStringList const& filePaths, QStringList* notAdded /*= 0*/);
    bool removeFiles(QStringList const& filePaths, QStringList* notRemoved /*= 0*/);
    bool deleteFiles(QStringList const& filePaths);
    bool renameFile(QString const& filePath, QString const& newFilePath);
    QList<ProjectExplorer::RunConfiguration*> runConfigurationsFor(Node* node);

private:
    Project* project_;
    Core::IDocument* projectFile_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECTNODE_HPP_INCLUDED
