#include "bbproject.hpp"
#include "bbprojectfile.hpp"
#include "bbprojectmanager.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbprojectnode.hpp"
#include <coreplugin/icontext.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
#include <QDir>
#include <QFileInfo>

namespace BoostBuildProjectManager {
namespace Internal {

Project::Project(ProjectManager* manager, QString const& fileName)
    : manager_(manager)
    , fileName_(fileName)
    , projectFile_(new ProjectFile(this, fileName_))
    , projectNode_(new ProjectNode(this, projectFile_))
{
    Q_ASSERT(manager_);
    Q_ASSERT(!fileName_.isEmpty());

    setId(Constants::PROJECT_ID);
    setProjectContext(Core::Context(Constants::PROJECT_CONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    QFileInfo const fileInfo(fileName_);
    QDir const dir(fileInfo.dir());

    projectName_ = fileInfo.absoluteDir().dirName();
    // TODO: String const displayName(projectName_ + " (" + fileInfo.completeBaseName() + ")");
    filesFileName_ = QFileInfo(dir, fileName_ + QLatin1String(".files")).absoluteFilePath();

    projectNode_->setDisplayName(projectName_);

    manager_->registerProject(this);
}

Project::~Project()
{
    manager_->unregisterProject(this);
    delete projectNode_;
}

QString Project::displayName() const
{
    return projectName_;
}

Core::IDocument* Project::document() const
{
    return projectFile_;
}

ProjectExplorer::IProjectManager* Project::projectManager() const
{
    return manager_;
}

ProjectExplorer::ProjectNode* Project::rootProjectNode() const
{
    return projectNode_;
}

QStringList Project::files(FilesMode fileMode) const
{
    Q_UNUSED(fileMode);
    return files_;
}

bool Project::needsConfiguration() const
{
    // TODO: Does Boost.Build project require any configuration on loading?
    //       - b2/bjam command lookup
    //       - targets listing
    return false;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
