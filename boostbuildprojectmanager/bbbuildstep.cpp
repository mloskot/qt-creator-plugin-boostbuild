//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// Copyright (C) 2013 Openismus GmbH.
//
#include "bbbuildconfiguration.hpp"
#include "bbbuildstep.hpp"
#include "bboutputparser.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <extensionsystem/pluginmanager.h>
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
#include <utils/qtcprocess.h>
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
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl, BuildStep* bs)
    : AbstractProcessStep(bsl, bs)
    , tasks_(bs->tasks_)
    , arguments_(bs->arguments_)
{
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl, Core::Id const id)
    : AbstractProcessStep(bsl, id)
{
}

bool BuildStep::init()
{
    setDefaultDisplayName(QLatin1String(Constants::BOOSTBUILD));

    tasks_.clear();
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

        tasks_.append(task);
        return !tasks_.empty(); // otherwise the tasks will not get reported
    }

    ProjectExplorer::BuildConfiguration* bc = buildConfiguration();
    if (!bc)
        bc = target()->activeBuildConfiguration();
    QTC_ASSERT(bc, return false);

    setIgnoreReturnValue(Constants::ReturnValueNotIgnored);

    // TODO: we're waiting for Jamfile parser or b2 --list-targets feature
    //QString arguments = Utils::QtcProcess::joinArgs(buildTargets_);
    //Utils::QtcProcess::addArgs(&arguments, additionalArguments());

    ProjectExplorer::ProcessParameters* pp = processParameters();
    pp->setMacroExpander(bc->macroExpander());
    {
        // from GenericProjectManager code:
        // Force output to english for the parsers.
        // Do this here and not in the toolchain's addToEnvironment() to not
        // screw up the users run environment.
        Utils::Environment env = bc->environment();
        env.set(QLatin1String("LC_ALL"), QLatin1String("C"));
        pp->setEnvironment(env);
    }
    pp->setWorkingDirectory(bc->buildDirectory().toString());
    pp->setCommand(makeCommand(bc->environment()));
    pp->setArguments(allArguments());
    pp->resolveAll();

    // Create Boost.Build parser and chain with existing parsers
    setOutputParser(new BoostBuildParser());
    if (ProjectExplorer::IOutputParser* parser = target()->kit()->createOutputParser())
        appendOutputParser(parser);
    outputParser()->setWorkingDirectory(pp->effectiveWorkingDirectory());

    BBPM_QDEBUG(displayName() << ", " << bc->buildDirectory().toString()
                << ", " << pp->effectiveWorkingDirectory());

    return ProjectExplorer::AbstractProcessStep::init();
}

void BuildStep::run(QFutureInterface<bool>& fi)
{
    BBPM_QDEBUG("running: " << displayName());

    bool canContinue = true;
    foreach (ProjectExplorer::Task const& t, tasks_)
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

QVariantMap BuildStep::toMap() const
{
    QVariantMap map(ProjectExplorer::AbstractProcessStep::toMap());
    //TODO: BBPM_QDEBUG("TODO");
    return map;
}

bool BuildStep::fromMap(QVariantMap const& map)
{
    BBPM_QDEBUG("TODO");
    // TODO: see CMakeProjectManager
    // TODO: additionalArguments = map.value(QLatin1String(BOOSTBUILD_ADDITIONAL_ARGUMENTS_KEY)).toString();
    return ProjectExplorer::BuildStep::fromMap(map);
}

QString BuildStep::makeCommand(Utils::Environment const& env) const
{
    BBPM_QDEBUG("TODO: bjam vs b2 selection");
    Q_UNUSED(env);

    return QLatin1String(Constants::COMMAND_BB2);
}

QString BuildStep::allArguments() const
{
    return Utils::QtcProcess::joinArgs(arguments_);
}

void BuildStep::appendArgument(QString const& arg)
{
    // TODO: find duplicates?
    arguments_.append(arg);
}

void BuildStep::setArguments(QString const& args)
{
    Utils::QtcProcess::SplitError err;
    QStringList argsList = Utils::QtcProcess::splitArgs(args, false, &err);
    if (err == Utils::QtcProcess::SplitOk)
    {
        arguments_ = argsList;
        emit argumentsChanged(args);
    }
}

ProjectExplorer::BuildConfiguration::BuildType
BuildStep::buildType() const
{
    return arguments_.contains(QLatin1String("variant=release"))
            ? BuildConfiguration::Release
            : BuildConfiguration::Debug;
}

void
BuildStep::setBuildType(ProjectExplorer::BuildConfiguration::BuildType type)
{
    // TODO: Move literals to constants
    QString arg(QLatin1String("variant="));
    if (type == BuildConfiguration::Release)
        arg += QLatin1String("release");
    else
        arg += QLatin1String("debug");

    appendArgument(arg);
}

BuildStepFactory::BuildStepFactory(QObject* parent)
    : IBuildStepFactory(parent)
{
}

/*static*/ BuildStepFactory* BuildStepFactory::getObject()
{
    return ExtensionSystem::PluginManager::getObject<BuildStepFactory>();
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
    BBPM_QDEBUG("id: " << id.toString());

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

BuildStep*
BuildStepFactory::create(ProjectExplorer::BuildStepList* parent)
{
    ProjectExplorer::BuildStep* step = create(parent, Constants::BUILDSTEP_ID);
    return qobject_cast<BuildStep*>(step);
}

ProjectExplorer::BuildStep*
BuildStepFactory::create(ProjectExplorer::BuildStepList* parent, Core::Id const id)
{
    BBPM_QDEBUG("id: " << id.toString());
    if (!canCreate(parent, id))
        return 0;

    BuildStep* step = new BuildStep(parent);

    if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_CLEAN)
        step->appendArgument(QLatin1String("--clean"));

    return step;
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
    return parent->target()->project()->id() == Constants::PROJECT_ID;
}

BuildStepConfigWidget::BuildStepConfigWidget(BuildStep* step)
    : step_(step)
{
    QFormLayout *fl = new QFormLayout(this);
    fl->setMargin(0);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    setLayout(fl);

        arguments_ = new QLineEdit(this);
    fl->addRow(tr("Arguments:"), arguments_);
    arguments_->setText(step_->allArguments());

    updateDetails();

    connect(arguments_, SIGNAL(textChanged(QString))
          , step, SLOT(setArguments(QString)));
    connect(step, SIGNAL(argumentsChanged(QString))
          , this, SLOT(updateDetails()));
    connect(step_->project(), SIGNAL(environmentChanged())
          , this, SLOT(updateDetails()));
}

BuildStepConfigWidget::~BuildStepConfigWidget()
{
}

QString BuildStepConfigWidget::displayName() const
{
    // TODO: return step_->displayName();
    return tr(Constants::BOOSTBUILD
            , "BoostBuildProjectManager::BuildStepConfigWidget display name.");
}

QString BuildStepConfigWidget::summaryText() const
{
    return summary_;
}

void BuildStepConfigWidget::setSummaryText(QString const& text)
{
    if (text != summary_)
    {
        summary_ = text;
        emit updateSummary();
    }
}

void BuildStepConfigWidget::updateDetails()
{
    ProjectExplorer::BuildConfiguration* bc = step_->buildConfiguration();
    if (!bc)
        bc = step_->target()->activeBuildConfiguration();
    Q_ASSERT(bc);

    ProjectExplorer::ToolChain* tc
        = ProjectExplorer::ToolChainKitInformation::toolChain(step_->target()->kit());
    if (!tc)
    {
        setSummaryText(tr("<b>Make:</b> %1")
            .arg(ProjectExplorer::ToolChainKitInformation::msgNoToolChainInTarget()));
        return;
    }

    // TODO
    //QString arguments = Utils::QtcProcess::joinArgs(step_->m_buildTargets);
    //Utils::QtcProcess::addArgs(&arguments, step_->additionalArguments());

    ProjectExplorer::ProcessParameters params;
    params.setMacroExpander(bc->macroExpander());
    params.setEnvironment(bc->environment());
    params.setWorkingDirectory(bc->buildDirectory().toString());
    params.setCommand(step_->makeCommand(bc->environment()));
    params.setArguments(step_->allArguments());

    if (params.commandMissing())
    {
        setSummaryText(tr("<b>Boost.Build:</b> %1 not found in the environment.")
            .arg(params.command())); // Override display text
    }
    else
    {
        setSummaryText(params.summaryInWorkdir(displayName()));
    }

}

} // namespace Internal
} // namespace BoostBuildProjectManager
