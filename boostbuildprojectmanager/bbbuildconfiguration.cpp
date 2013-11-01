#include "bbbuildconfiguration.hpp"
#include "bbprojectmanagerconstants.hpp"
// Qt Creator
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/target.h>
#include <utils/qtcassert.h>
// Qt
#include <QFileInfo>

namespace BoostBuildProjectManager {
namespace Internal {

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* parent)
    : ProjectExplorer::BuildConfiguration(
          parent, Core::Id(Constants::BUILDCONFIGURATION_ID))
{
}

BuildConfiguration::BuildConfiguration(
    ProjectExplorer::Target* parent
  , BuildConfiguration* source)
    : ProjectExplorer::BuildConfiguration(parent, source)
{
}

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* parent, Core::Id const id)
    : ProjectExplorer::BuildConfiguration(parent, id)
{
    // TODO: cloneSteps
}

NamedWidget *BuildConfiguration::createConfigWidget()
{
    // TODO
    return 0; //new BuildSettingsWidget(this);
}

BuildConfigurationFactory::BuildConfigurationFactory(QObject* parent)
    : IBuildConfigurationFactory(parent)
{
}

int BuildConfigurationFactory::priority(ProjectExplorer::Target const* parent) const
{
    return canHandle(parent) ? 0 : -1;
}

int BuildConfigurationFactory::priority(
    ProjectExplorer::Kit const* k
  , QString const& projectPath) const
{
    Core::MimeType const mt = Core::MimeDatabase::findByFile(QFileInfo(projectPath));
    return (k && mt.matchesType(QLatin1String(Constants::MIMETYPE_JAMFILE)))
            ? 0
            : -1;
}

QList<ProjectExplorer::BuildInfo*>
availableBuilds(ProjectExplorer::Target const* parent) const
{
    QList<ProjectExplorer::BuildInfo*> result;
    result << createBuildInfo(parent->kit()
                , Utils::FileName::fromString(parent->project()->projectDirectory()));
    return result;
}

QList<ProjectExplorer::BuildInfo*> BuildConfigurationFactory::availableSetups(
    ProjectExplorer::Kit const* k
  , QString const& projectPath) const
{
    // TODO:ProjectExplorer::Project::projectDirectory(projectPath)
    BuildInfo* info = createBuildInfo(k, Utils::FileName::fromString(AutotoolsProject::defaultBuildDirectory(projectPath)));
    //: The name of the build configuration created by default for a autotools project.
    info->displayName = tr("Default");
    //info->buildDirectory
    QList<BuildInfo*> result;
    result << info;
    return result;
}

ProjectExplorer::BuildConfiguration* BuildConfigurationFactory::create(
    ProjectExplorer::Target* parent
  , BuildInfo const* info) const
{
    QTC_ASSERT(parent, return 0);
    QTC_ASSERT(info->factory() == this, return 0);
    QTC_ASSERT(info->kitId == parent->kit()->id(), return 0);
    QTC_ASSERT(!info->displayName.isEmpty(), return 0);

    BuildConfiguration* bc = new BuildConfiguration(parent);
    bc->setDisplayName(info->displayName);
    bc->setDefaultDisplayName(info->displayName);
    bc->setBuildDirectory(info->buildDirectory);

    // TODO:
    ProjectExplorer::BuildStepList* buildSteps =
        bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    ProjectExplorer::BuildStepList* cleanSteps =
        bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_CLEAN);

    return bc;
}

bool BuildConfigurationFactory::canHandle(ProjectExplorer::Target const* t) const
{
    QTC_ASSERT(t, return false);

    return t->project()->supportsKit(t->kit())
            ? t->project()->id() == Constants::PROJECT_ID
            : false;
}

ProjectExplorer::BuildInfo*
BuildConfigurationFactory::createBuildInfo(
        ProjectExplorer::Kit const* k
      , Utils::FileName const& buildDir) const
{
    Q_ASSERT(k);

    ProjectExplorer::BuildInfo* info = new ProjectExplorer::BuildInfo(this);
    info->typeName = tr("Build");
    info->buildDirectory = buildDir;
    info->kitId = k->id();
    // TODO: environment, shadowbuild, sourcedirectory, BuildConfiguration::Release ?
    return info;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
