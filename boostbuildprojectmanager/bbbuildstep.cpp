#include "bbbuildconfiguration.hpp"
#include "bbbuildstep.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/processparameters.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectconfiguration.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <projectexplorer/toolchain.h>
#include <utils/environment.h>
#include <utils/qtcassert.h>
// Qt
#include <QFormLayout>
#include <QLineEdit>
#include <QString>
// std
#include <memory>

namespace BoostBuildProjectManager {
namespace Internal {

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl)
    : ProjectExplorer::AbstractProcessStep(bsl, Core::Id(Constants::BUILDSTEP_ID))
{
    setDefaultDisplayName(tr("Boost.Build"));
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl, BuildStep* bs)
    : AbstractProcessStep(bsl, bs)
{
    setDefaultDisplayName(tr("Boost.Build"));
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl, Core::Id const id)
    : AbstractProcessStep(bsl, id)
{
    setDefaultDisplayName(tr("Boost.Build"));
}

QString BuildStep::makeCommand(Utils::Environment const& env) const
{
    BBPM_QDEBUG("TODO: bjam vs b2 selection");
    Q_UNUSED(env);
    return QLatin1String(Constants::BB2_COMMAND);
}

bool BuildStep::fromMap(QVariantMap const& map)
{
    BBPM_QDEBUG("TODO");
    // TODO: see CMakeProjectManager
    // TODO: madditionalArguments = map.value(QLatin1String(AUTOGEN_ADDITIONAL_ARGUMENTS_KEY)).toString();
    return ProjectExplorer::BuildStep::fromMap(map);
}

bool BuildStep::init()
{
    m_tasks.clear();
    ProjectExplorer::ToolChain* tc =
        ProjectExplorer::ToolChainKitInformation::toolChain(target()->kit());
    if (!tc)
    {
        BBPM_QDEBUG("Qt Creator needs compiler");
        typedef ProjectExplorer::Task Task;
        Task task(Task::Error
             , tr("Qt Creator needs a compiler set up to build. Configure a compiler in the kit options.")
             , Utils::FileName(), -1
             , ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM);

        m_tasks.append(task);
        return !m_tasks.empty(); // otherwise the tasks will not get reported
    }

    ProjectExplorer::BuildConfiguration* bc = buildConfiguration();
    ProjectExplorer::ProcessParameters* pp = processParameters();
    pp->setMacroExpander(bc->macroExpander());
    pp->setEnvironment(bc->environment());
    pp->setWorkingDirectory(bc->buildDirectory().toString());
    pp->setCommand(makeCommand(bc->environment()));
    pp->setArguments(additionalArguments());
    pp->resolveAll();

    BBPM_QDEBUG(displayName() << ", " << bc->buildDirectory().toString());
    return ProjectExplorer::AbstractProcessStep::init();
}

void BuildStep::run(QFutureInterface<bool>& fi)
{
    BBPM_QDEBUG("running: " << displayName());

    bool canContinue = true;
    foreach (ProjectExplorer::Task const& t, m_tasks)
    {
        addTask(t);
        canContinue = false;
    }

    if (!canContinue)
    {
        emit addOutput(tr("Configuration is faulty. Check the Issues view for details.")
                     , BuildStep::MessageOutput);
        fi.reportResult(false);
        emit finished();
    }
    else
    {
        AbstractProcessStep::run(fi);
    }
}

ProjectExplorer::BuildStepConfigWidget* BuildStep::createConfigWidget()
{
    return new BuildStepConfigWidget(this);
}

bool BuildStep::immutable() const
{
    return false;
}

QString BuildStep::additionalArguments() const
{
    BBPM_QDEBUG("TODO");
    return QString();
}

QVariantMap BuildStep::toMap() const
{
    BBPM_QDEBUG("TODO");
    return QVariantMap();
}

void BuildStep::setStepType(StepType type)
{
    m_stepType = type;
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

QString BuildStepFactory::displayNameForId(Core::Id const id) const
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
    BBPM_QDEBUG("TODO");
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
    BBPM_QDEBUG("TODO");
    QFormLayout *fl = new QFormLayout(this);
    fl->setMargin(0);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    setLayout(fl);

    arguments_ = new QLineEdit(this);
    fl->addRow(tr("Arguments:"), arguments_);
    arguments_->setText(step_->additionalArguments());

    updateDetails();

    connect(arguments_, SIGNAL(textChanged(QString)),
            step, SLOT(setAdditionalArguments(QString)));
    connect(step, SIGNAL(additionalArgumentsChanged(QString)),
            this, SLOT(updateDetails()));
    connect(step_->project(), SIGNAL(environmentChanged()),
            this, SLOT(updateDetails()));
}

BuildStepConfigWidget::~BuildStepConfigWidget()
{
}

QString BuildStepConfigWidget::displayName() const
{
    // TODO: return step_->displayName();
    return tr("Boost.Build"
            , "BoostBuildProjectManager::BuildStepConfigWidget display name.");
}

QString BuildStepConfigWidget::summaryText() const
{
    return summary_;
}

void BuildStepConfigWidget::updateDetails()
{
    ProjectExplorer::BuildConfiguration* bc = step_->buildConfiguration();
    if (!bc)
        bc = step_->target()->activeBuildConfiguration();
    Q_ASSERT(bc);

    ProjectExplorer::ToolChain* tc =
            ProjectExplorer::ToolChainKitInformation::toolChain(step_->target()->kit());

    if (tc)
    {
        // TODO
        //QString arguments = Utils::QtcProcess::joinArgs(step_->m_buildTargets);
        //Utils::QtcProcess::addArgs(&arguments, step_->additionalArguments());

        ProjectExplorer::ProcessParameters params;
        params.setMacroExpander(bc->macroExpander());
        params.setEnvironment(bc->environment());
        params.setWorkingDirectory(bc->buildDirectory().toString());
        params.setCommand(step_->makeCommand(bc->environment()));
        //params.setArguments(arguments);
        summary_ = params.summary(displayName());
    }
    else
    {
        summary_ = QLatin1String("<b>")
                 + ProjectExplorer::ToolChainKitInformation::msgNoToolChainInTarget()
                 + QLatin1String("</b>");
    }

    emit updateSummary();
}

} // namespace Internal
} // namespace BoostBuildProjectManager
