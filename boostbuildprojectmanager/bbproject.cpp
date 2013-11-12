#include "bbbuildconfiguration.hpp"
#include "bbbuildstep.hpp"
#include "bbproject.hpp"
#include "bbprojectfile.hpp"
#include "bbprojectmanager.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbprojectnode.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/target.h>
#include <qtsupport/customexecutablerunconfiguration.h>
// Qt
#include <QDir>
#include <QFileInfo>
// std
#include <memory>

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
    filesFileName_ = QFileInfo(dir, fileName_ + QLatin1String(".files")).absoluteFilePath();

    projectNode_->setDisplayName(projectName_);

    manager_->registerProject(this);

    BBPM_QDEBUG("created project: "
                << displayName()
                << " (" << fileInfo.completeBaseName() << ")");
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
    BBPM_QDEBUG(displayName() << "has" << files_.size() << "files");

    Q_UNUSED(fileMode);
    return files_;
}

bool Project::needsConfiguration() const
{
    // TODO:
    // Does Boost.Build project require any configuration on loading?
    // - Kit selection
    // - build/stage directory
    // - targets listing
    // CMakeProjectManager seems to request configuration in fromMap()

    return false;
}

// This function is called at the very beginning to restore the settings
// from .user file, if there is any with previous settings stored.
bool Project::fromMap(QVariantMap const& map)
{
    BBPM_QDEBUG(displayName());

    if (!ProjectExplorer::Project::fromMap(map))
        return false;

    // NOTE:
    // Call setActiveBuildConfiguration when creating new build configurations.

    if (!activeTarget())
    {
        // Configure project from scratch

        // TODO: do we need on-loading wizard similar to CMakeProjectManager?
        ProjectExplorer::Kit* defaultKit = ProjectExplorer::KitManager::defaultKit();
        ProjectExplorer::Target* target= createTarget(defaultKit);
        addTarget(target);
    }
    else
    {
        // Configure project from settings sorced from .user file
        BBPM_QDEBUG(displayName() << "has user file");
    }

    // Sanity check (taken from GenericProjectManager):
    // We need both a BuildConfiguration and a RunConfiguration!
    QList<ProjectExplorer::Target*> targetList = targets();
    foreach (ProjectExplorer::Target* t, targetList)
    {
        if (!t->activeBuildConfiguration())
        {
            removeTarget(t);
            continue;
        }
        if (!t->activeRunConfiguration())
            t->addRunConfiguration(
                        new QtSupport::CustomExecutableRunConfiguration(t));
    }

    // TODO: refresh(Everything);
    QTC_ASSERT(hasActiveBuildSettings(), return false);
    return activeTarget() != 0;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
