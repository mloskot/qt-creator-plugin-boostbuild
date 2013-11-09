#include "bbproject.hpp"
#include "bbprojectfile.hpp"
#include "bbprojectmanager.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbprojectnode.hpp"
// Qt Creator
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
// Qt
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

Core::Id Project::id() const
{
    return Core::Id(Constants::PROJECT_ID);
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

// This function is called at the very beginning to restore the settings
// from .user file, if there is any with previous settings stored.
bool Project::fromMap(QVariantMap const& map)
{
    if (!ProjectExplorer::Project::fromMap(map))
        return false;

    // TODO:  see autotools
    // TODO: Load the project tree structure.

    // Create target, if necessary
    if (!activeTarget())
    {
        if (ProjectExplorer::Kit* defaultKit = ProjectExplorer::KitManager::defaultKit())
            addTarget(createTarget(defaultKit));
    }

    return activeTarget() != 0;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
