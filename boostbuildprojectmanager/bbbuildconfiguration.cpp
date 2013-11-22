#include "bbbuildconfiguration.hpp"
#include "bbbuildinfo.hpp"
#include "bbbuildstep.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/project.h>
#include <projectexplorer/namedwidget.h>
#include <projectexplorer/target.h>
#include <utils/qtcassert.h>
// Qt
#include <QFileInfo>
#include <QString>
// std
#include <memory>

namespace BoostBuildProjectManager {
namespace Internal {

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* parent)
    : ProjectExplorer::BuildConfiguration(
          parent, Core::Id(Constants::BUILDCONFIGURATION_ID))
{
    BBPM_QDEBUG("TODO");
}

BuildConfiguration::BuildConfiguration(
    ProjectExplorer::Target* parent
  , BuildConfiguration* source)
    : ProjectExplorer::BuildConfiguration(parent, source)
{
    BBPM_QDEBUG("TODO");
}

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* parent
                                     , Core::Id const id)
    : ProjectExplorer::BuildConfiguration(parent, id)
{
    BBPM_QDEBUG("TODO");
}

ProjectExplorer::NamedWidget*
BuildConfiguration::createConfigWidget()
{
    BBPM_QDEBUG("TODO");
    return 0; //new BuildSettingsWidget(this);
}

BuildConfiguration::BuildType
BuildConfiguration::buildType() const
{
    BuildType type = Unknown;

    ProjectExplorer::BuildStepList* buildStepList
        = stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
    Q_ASSERT(buildStepList);
    foreach (ProjectExplorer::BuildStep* bs, buildStepList->steps())
    {
        if (BuildStep* bbStep = qobject_cast<BuildStep*>(bs))
        {
            type = bbStep->buildType();
            break;
        }
    }

    return type;
}

BuildConfigurationFactory::BuildConfigurationFactory(QObject* parent)
    : IBuildConfigurationFactory(parent)
{
}

int
BuildConfigurationFactory::priority(ProjectExplorer::Target const* parent) const
{
    return canHandle(parent) ? 0 : -1;
}

int
BuildConfigurationFactory::priority(
    ProjectExplorer::Kit const* k
  , QString const& projectPath) const
{
    BBPM_QDEBUG(k->displayName() << ", " << projectPath);

    Core::MimeType const mt = Core::MimeDatabase::findByFile(QFileInfo(projectPath));
    return (k && mt.matchesType(QLatin1String(Constants::MIMETYPE_JAMFILE)))
            ? 0
            : -1;
}

QList<ProjectExplorer::BuildInfo*>
BuildConfigurationFactory::availableBuilds(ProjectExplorer::Target const* parent) const
{
    BBPM_QDEBUG("target: " << parent->displayName());

    ProjectExplorer::Project* project = parent->project();
    QString const projectPath(project->projectDirectory());
    BBPM_QDEBUG(projectPath);

    QList<ProjectExplorer::BuildInfo*> result;
    result << createBuildInfo(parent->kit(), projectPath, BuildConfiguration::Debug);
    return result;
}

QList<ProjectExplorer::BuildInfo*>
BuildConfigurationFactory::availableSetups(
    ProjectExplorer::Kit const* k
  , QString const& projectPath) const
{
    BBPM_QDEBUG(projectPath);

    QList<ProjectExplorer::BuildInfo*> result;
    result << createBuildInfo(k, projectPath, BuildConfiguration::Debug);
    result << createBuildInfo(k, projectPath, BuildConfiguration::Release);
    return result;
}

ProjectExplorer::BuildConfiguration*
BuildConfigurationFactory::create(
    ProjectExplorer::Target* parent
  , ProjectExplorer::BuildInfo const* info) const
{
    QTC_ASSERT(parent, return 0);
    QTC_ASSERT(info->factory() == this, return 0);
    QTC_ASSERT(info->kitId == parent->kit()->id(), return 0);
    QTC_ASSERT(!info->displayName.isEmpty(), return 0);

    BBPM_QDEBUG(info->displayName);

    BuildInfo const* bi = static_cast<BuildInfo const*>(info);
    BuildConfiguration* bc = new BuildConfiguration(parent);
    bc->setDisplayName(bi->displayName);
    bc->setDefaultDisplayName(bi->displayName);
    bc->setBuildDirectory(bi->buildDirectory);

    // TODO: check Jamfile/Jamroot exists
    // Q_ASSERT(QFile(parent->project()->projectDirectory() + QLatin1String("/Jamfile.v2"));

    BuildStepFactory* stepFactory = BuildStepFactory::getObject();
    QTC_ASSERT(stepFactory, return 0);

    // Build steps
    if (ProjectExplorer::BuildStepList* buildSteps =
            bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD))
    {
        BuildStep* step = stepFactory->create(buildSteps);
        QTC_ASSERT(step, return 0);
        step->setBuildType(bi->buildType);
        buildSteps->insertStep(0, step);
    }

    // Clean steps
    if (ProjectExplorer::BuildStepList* cleanSteps =
            bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_CLEAN))
    {
        BuildStep* step = stepFactory->create(cleanSteps);
        QTC_ASSERT(step, return 0);
        step->setBuildType(bi->buildType);
        cleanSteps->insertStep(0, step);
    }

    return bc;
}

bool
BuildConfigurationFactory::canClone(ProjectExplorer::Target const* parent
       , ProjectExplorer::BuildConfiguration* source) const
{
    Q_ASSERT(parent);
    Q_ASSERT(source);

    return canHandle(parent)
            ? source->id() == Constants::BUILDCONFIGURATION_ID
            : false;
}

BuildConfiguration*
BuildConfigurationFactory::clone(ProjectExplorer::Target* parent
                               , ProjectExplorer::BuildConfiguration* source)
{
    Q_ASSERT(parent);
    Q_ASSERT(source);

    BuildConfiguration* copy = 0;
    if (canClone(parent, source))
    {
        BuildConfiguration* old = static_cast<BuildConfiguration*>(source);
        copy = new BuildConfiguration(parent, old);
    }
    return copy;
}

bool
BuildConfigurationFactory::canRestore(ProjectExplorer::Target const* parent
                                    , QVariantMap const& map) const
{
    Q_ASSERT(parent);

    return canHandle(parent)
            ? ProjectExplorer::idFromMap(map) == Constants::BUILDCONFIGURATION_ID
            : false;
}

BuildConfiguration*
BuildConfigurationFactory::restore(ProjectExplorer::Target *parent
                                 , QVariantMap const& map)
{
    Q_ASSERT(parent);

    if (canRestore(parent, map))
    {
        std::unique_ptr<BuildConfiguration> bc(new BuildConfiguration(parent));
        if (bc->fromMap(map))
            return bc.release();
    }
    return 0;
}

bool
BuildConfigurationFactory::canHandle(ProjectExplorer::Target const* t) const
{
    QTC_ASSERT(t, return false);

    return t->project()->supportsKit(t->kit())
            ? t->project()->id() == Constants::PROJECT_ID
            : false;
}

BuildInfo*
BuildConfigurationFactory::createBuildInfo(
    ProjectExplorer::Kit const* k
  , QString const& projectPath
  , BuildConfiguration::BuildType type) const
{
    Q_ASSERT(k);

    BuildInfo* info = new BuildInfo(this);
    info->typeName = tr("Build");
    if (type == BuildConfiguration::Release)
        info->displayName = tr("Release");
    else
        info->displayName = tr("Debug");
    info->buildType = type;
    info->buildDirectory = defaultBuildDirectory(projectPath);
    info->kitId = k->id();
    info->supportsShadowBuild = true;
    // TODO: use bjam vs use b2 command

    BBPM_QDEBUG(info->typeName << " in " << projectPath);
    return info;
}

Utils::FileName
BuildConfigurationFactory::defaultBuildDirectory(QString const& projectPath) const
{
    BBPM_QDEBUG(projectPath);

    return Utils::FileName::fromString(
        ProjectExplorer::Project::projectDirectory(projectPath));
}

} // namespace Internal
} // namespace BoostBuildProjectManager
