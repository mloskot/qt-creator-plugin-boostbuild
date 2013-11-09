#include "bbbuildstep.hpp"
#include "bbprojectmanagerconstants.hpp"
// Qt Creator
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectconfiguration.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <utils/qtcassert.h>
// Qt
#include <QFormLayout>
#include <QString>
// std
#include <memory>

namespace BoostBuildProjectManager {
namespace Internal {

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl)
    : ProjectExplorer::AbstractProcessStep(bsl, Core::Id(Constants::BUILDSTEP_ID))
{
    //setDefaultDisplayName(tr(""));
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl, BuildStep* bs)
    : AbstractProcessStep(bsl, bs)
{
    //setDefaultDisplayName(tr(""));
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl, Core::Id const id)
    : AbstractProcessStep(bsl, id)
{
    //setDefaultDisplayName(tr(""));
}

bool BuildStep::fromMap(QVariantMap const& map)
{
    // TODO: madditionalArguments = map.value(QLatin1String(AUTOGEN_ADDITIONAL_ARGUMENTS_KEY)).toString();
    return ProjectExplorer::BuildStep::fromMap(map);
}

bool BuildStep::init()
{
    ProjectExplorer::BuildConfiguration *bc = buildConfiguration();

    ProjectExplorer::ProcessParameters *pp = processParameters();
    pp->setMacroExpander(bc->macroExpander());
    pp->setEnvironment(bc->environment());
    pp->setWorkingDirectory(bc->buildDirectory().toString());
    pp->setCommand(QLatin1String(Constants::BB2_COMMAND)); // TODO: configuragle
    pp->setArguments(additionalArguments());
    pp->resolveAll();

    return ProjectExplorer::AbstractProcessStep::init();
}

void BuildStep::run(QFutureInterface<bool>& interface)
{
    (void)interface;
}

ProjectExplorer::BuildStepConfigWidget* BuildStep::createConfigWidget()
{
    return new BuildStepConfigWidget(this);
}

bool BuildStep::immutable() const
{
    QTC_ASSERT(0, return false);
}

QString BuildStep::additionalArguments() const
{
    QTC_ASSERT(0, return QString());
}

QVariantMap BuildStep::toMap() const
{
    QTC_ASSERT(0, return QVariantMap());
}

BuildStepFactory::BuildStepFactory(QObject* parent)
    : IBuildStepFactory(parent)
{
}

QList<Core::Id>
BuildStepFactory::availableCreationIds(ProjectExplorer::BuildStepList* parent) const
{
    return canHandle(parent)
        ? QList<Core::Id>() << Core::Id(Constants::BUILDSTEP_ID)
        : QList<Core::Id>();
}

QString BuildStepFactory::displayNameForId(const Core::Id id) const
{
    QString name;
    if (id == Constants::BUILDSTEP_ID)
    {
        name = tr("Boost.Build"
                , "Display name for BoostBuildProjectManager::BuildStep id.");
    }
    return name;
}

bool BuildStepFactory::canCreate(ProjectExplorer::BuildStepList* parent
                               , Core::Id const id) const
{
    return canHandle(parent) && Core::Id(Constants::BUILDSTEP_ID) == id;
}

ProjectExplorer::BuildStep*
BuildStepFactory::create(ProjectExplorer::BuildStepList* parent, Core::Id const id)
{
    return canCreate(parent, id) ? new BuildStep(parent) : 0;
}

bool BuildStepFactory::canClone(ProjectExplorer::BuildStepList *parent
                              , ProjectExplorer::BuildStep *source) const
{
    return canCreate(parent, source->id());
}

ProjectExplorer::BuildStep*
BuildStepFactory::clone(ProjectExplorer::BuildStepList* parent
                      , ProjectExplorer::BuildStep* source)
{
    return canClone(parent, source)
            ? new BuildStep(parent, static_cast<BuildStep*>(source))
            : 0;
}

bool BuildStepFactory::canRestore(ProjectExplorer::BuildStepList* parent
                                , QVariantMap const& map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::BuildStep*
BuildStepFactory::restore(ProjectExplorer::BuildStepList* parent
                        , QVariantMap const& map)
{
    Q_ASSERT(parent);

    if (canRestore(parent, map))
    {
        std::unique_ptr<BuildStep> bs(new BuildStep(parent));
        if (bs->fromMap(map))
            return bs.release();
    }
    return 0;
}

bool BuildStepFactory::canHandle(ProjectExplorer::BuildStepList* parent) const
{
    QTC_ASSERT(parent, return false);

    return parent->target()->project()->id() == Constants::PROJECT_ID
        && parent->id() == ProjectExplorer::Constants::BUILDSTEPS_BUILD;
}

BuildStepConfigWidget::BuildStepConfigWidget(BuildStep* step)
    : step_(step)
{
    QFormLayout *fl = new QFormLayout(this);
    fl->setMargin(0);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    setLayout(fl);
}

BuildStepConfigWidget::~BuildStepConfigWidget()
{

}

QString BuildStepConfigWidget::displayName() const
{
    // TODO tr("Boost.Build", "BoostBuildProjectManager::BuildStepConfigWidget display name.");
    return step_->displayName();
}

QString BuildStepConfigWidget::summaryText() const
{
    return summary_;
}


} // namespace Internal
} // namespace BoostBuildProjectManager
