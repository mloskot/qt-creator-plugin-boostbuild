#include "bbbuildstep.hpp"
#include "bbprojectmanagerconstants.hpp"
// Qt Creator
#include <projectexplorer/buildsteplist.h>
// Qt
#include <QString>

namespace BoostBuildProjectManager {
namespace Internal {

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
