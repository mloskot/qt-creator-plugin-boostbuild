#include "bbbuildstep.hpp"
#include "bbprojectmanagerconstants.hpp"
// Qt Creator
#include <projectexplorer/buildsteplist.h>
// Qt
#include <QString>

namespace BoostBuildProjectManager {
namespace Internal {

BuildStepFactory::BuildStepFactory(QObject* parent)
    : IBuildStepFactory(parent)
{
}

QList<Core::Id>
BuildStepFactory::availableCreationIds(ProjectExplorer::BuildStepList* bc) const
{
    return canHandle(parent)
        ? QList<Core::Id>() << Core::Id(Constants::BUILDSTEP_ID)
        : QList<Core::Id>();
}

QString BuildStepFactory::displayNameForId(const Core::Id id) const
{
    QString name;
    if (id == BUILDSTEP_ID)
    {
        name = tr("Boost.Build"
                , "Display name for BoostBuildProjectManager::BuildStep id.");
    }
    return name;
}

bool BuildStepFactory::canCreate(ProjectExplorer::BuildStepList* parent
                               , Core::Id const id) const
{
}

ProjectExplorer::BuildStep*
BuildStepFactory::create(ProjectExplorer::BuildStepList* parent, Core::Id const id)
{
}

bool BuildStepFactory::canClone(ProjectExplorer::BuildStepList *parent
                              , ProjectExplorer::BuildStep *source) const
{
}

ProjectExplorer::BuildStep*
BuildStepFactory::clone(ProjectExplorer::BuildStepList* parent
                      , ProjectExplorer::BuildStep* source);

bool BuildStepFactory::canRestore(ProjectExplorer::BuildStepList* parent
                                , QVariantMap const& map) const
{
}

ProjectExplorer::BuildStep*
BuildStepFactory::restore(ProjectExplorer::BuildStepList* parent
                        , QVariantMap const& map)
{

}

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl)
    : ProjectExplorer::AbstractProcessStep(bsl, Core::Id(Constants::BUILDSTEP_ID))
{
}

bool BuildStep::init()
{
    ProjectExplorer::BuildConfiguration *bc = buildConfiguration();

    ProjectExplorer::ProcessParameters *pp = processParameters();
    pp->setMacroExpander(bc->macroExpander());
    pp->setEnvironment(bc->environment());
    pp->setWorkingDirectory(bc->buildDirectory().toString());
    pp->setCommand(Constants::BB2_COMMAND); // TODO: configuragle
    pp->setArguments(additionalArguments());
    pp->resolveAll();

    return ProjectExplorer::AbstractProcessStep::init();
}

void BuildStep::run(QFutureInterface<bool>& interface)
{
}

ProjectExplorer::BuildStepConfigWidget* BuildStep::createConfigWidget()
{
}

bool BuildStep::immutable() const
{
}

QString BuildStep::additionalArguments() const
{
}

QVariantMap BuildStep::toMap() const
{
}

} // namespace Internal
} // namespace BoostBuildProjectManager
