//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
// This file is part of Qt Creator Boost.Build plugin project.
//
// This is free software; you can redistribute and/or modify it under
// the terms of the  GNU Lesser General Public License, Version 2.1
// as published by the Free Software Foundation.
// See accompanying file LICENSE.txt or copy at 
// http://www.gnu.org/licenses/lgpl-2.1-standalone.html.
//
#include "bbbuildconfiguration.hpp"
#include "bbbuildinfo.hpp"
#include "bbbuildstep.hpp"
#include "bbproject.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <coreplugin/icore.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/project.h>
#include <projectexplorer/namedwidget.h>
#include <projectexplorer/target.h>
#include <utils/pathchooser.h>
#include <utils/fileutils.h>
#include <utils/qtcassert.h>
#include <utils/mimetypes/mimedatabase.h>

// Qt
#include <QFileInfo>
#include <QFormLayout>
#include <QInputDialog>
#include <QScopedPointer>
#include <QString>
// std
#include <memory>

namespace BoostBuildProjectManager {
namespace Internal {

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* parent)
    : ProjectExplorer::BuildConfiguration(parent
        , Core::Id(Constants::BUILDCONFIGURATION_ID))
{
    ProjectExplorer::Project const* p = parent->project();
    Q_ASSERT(p);
    setWorkingDirectory(p->projectDirectory());
}

BuildConfiguration::BuildConfiguration(
    ProjectExplorer::Target* parent
  , BuildConfiguration* source)
    : ProjectExplorer::BuildConfiguration(parent, source)
{
    if (BuildConfiguration* bc = qobject_cast<BuildConfiguration*>(source))
        setWorkingDirectory(bc->workingDirectory());
}

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* parent, Core::Id const id)
    : ProjectExplorer::BuildConfiguration(parent, id)
{
    ProjectExplorer::Project const* p = parent->project();
    Q_ASSERT(p);
    setWorkingDirectory(p->projectDirectory());
}

QVariantMap BuildConfiguration::toMap() const
{
    QVariantMap map(ProjectExplorer::BuildConfiguration::toMap());
    map.insert(QLatin1String(Constants::BC_KEY_WORKDIR), workingDirectory_.toString());
    return map;
}

bool BuildConfiguration::fromMap(QVariantMap const& map)
{
    if (!ProjectExplorer::BuildConfiguration::fromMap(map))
        return false;

    QString dir = map.value(QLatin1String(Constants::BC_KEY_WORKDIR)).toString();
    setWorkingDirectory(Utils::FileName::fromString(dir));

    return true;
}

ProjectExplorer::NamedWidget*
BuildConfiguration::createConfigWidget()
{
    return new BuildSettingsWidget(this);
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

Utils::FileName BuildConfiguration::workingDirectory() const
{
    Q_ASSERT(!workingDirectory_.isEmpty());
    return workingDirectory_;
}

void BuildConfiguration::setWorkingDirectory(Utils::FileName const& dir)
{
    if (dir.isEmpty())
    {
        if (ProjectExplorer::Target* t = target())
        {
            QString const dwd
                = Project::defaultWorkingDirectory(t->project()->projectDirectory().toString());
            workingDirectory_ = Utils::FileName::fromString(dwd);
        }
    }
    else
    {
        workingDirectory_ = dir;
    }

    Q_ASSERT(!workingDirectory_.isEmpty());
    emitWorkingDirectoryChanged();
}

void BuildConfiguration::emitWorkingDirectoryChanged()
{
    if (workingDirectory() != lastEmmitedWorkingDirectory_)
    {
        lastEmmitedWorkingDirectory_= workingDirectory();
        emit workingDirectoryChanged();
    }
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
BuildConfigurationFactory::priority(ProjectExplorer::Kit const* k
    , QString const& projectPath) const
{
    BBPM_QDEBUG(k->displayName() << ", " << projectPath);

    Utils::MimeDatabase mdb;
    Utils::MimeType mimeType = mdb.mimeTypeForFile(QFileInfo(projectPath));

    return (k && mimeType.matchesName(QLatin1String(Constants::MIMETYPE_JAMFILE)))
            ? 0
            : -1;
}

QList<ProjectExplorer::BuildInfo*>
BuildConfigurationFactory::availableBuilds(ProjectExplorer::Target const* parent) const
{
    BBPM_QDEBUG("target: " << parent->displayName());

    ProjectExplorer::Project* project = parent->project();
    QString const projectPath(project->projectDirectory().toString());
    BBPM_QDEBUG(projectPath);

    QList<ProjectExplorer::BuildInfo*> result;
    result << createBuildInfo(parent->kit(), projectPath, BuildConfiguration::Debug);
    result << createBuildInfo(parent->kit(), projectPath, BuildConfiguration::Release);
    return result;
}

QList<ProjectExplorer::BuildInfo*>
BuildConfigurationFactory::availableSetups(ProjectExplorer::Kit const* k
    , QString const& projectPath) const
{
    BBPM_QDEBUG(projectPath);

    QList<ProjectExplorer::BuildInfo*> result;
    result << createBuildInfo(k, projectPath, BuildConfiguration::Debug);
    result << createBuildInfo(k, projectPath, BuildConfiguration::Release);
    return result;
}

ProjectExplorer::BuildConfiguration*
BuildConfigurationFactory::create(ProjectExplorer::Target* parent
    , ProjectExplorer::BuildInfo const* info) const
{
    QTC_ASSERT(parent, return 0);
    QTC_ASSERT(info->factory() == this, return 0);
    QTC_ASSERT(info->kitId == parent->kit()->id(), return 0);
    QTC_ASSERT(!info->displayName.isEmpty(), return 0);
    BBPM_QDEBUG(info->displayName);

    Project* project = qobject_cast<Project*>(parent->project());
    QTC_ASSERT(project, return 0);

    BuildInfo const* bi = static_cast<BuildInfo const*>(info);
    QScopedPointer<BuildConfiguration> bc(new BuildConfiguration(parent));
    bc->setDisplayName(bi->displayName);
    bc->setDefaultDisplayName(bi->displayName);
    bc->setBuildDirectory(bi->buildDirectory);
    bc->setWorkingDirectory(bi->workingDirectory);

    BuildStepFactory* stepFactory = BuildStepFactory::getObject();
    QTC_ASSERT(stepFactory, return 0);

    // Build steps
    if (ProjectExplorer::BuildStepList* buildSteps
            = bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD))
    {
        BuildStep* step = stepFactory->create(buildSteps);
        QTC_ASSERT(step, return 0);
        step->setBuildType(bi->buildType);
        buildSteps->insertStep(0, step);
    }

    // Clean steps
    if (ProjectExplorer::BuildStepList* cleanSteps
            = bc->stepList(ProjectExplorer::Constants::BUILDSTEPS_CLEAN))
    {
        BuildStep* step = stepFactory->create(cleanSteps);
        QTC_ASSERT(step, return 0);
        step->setBuildType(bi->buildType);
        cleanSteps->insertStep(0, step);
    }

    return bc.take();
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
        QScopedPointer<BuildConfiguration> bc(new BuildConfiguration(parent));
        if (bc->fromMap(map))
            return bc.take();
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
BuildConfigurationFactory::createBuildInfo(ProjectExplorer::Kit const* k
    , QString const& projectPath
    , BuildConfiguration::BuildType type) const
{
    Q_ASSERT(k);

    BuildInfo* info = new BuildInfo(this);
    if (type == BuildConfiguration::Release)
        info->displayName = tr("Release");
    else
        info->displayName = tr("Debug");
    info->typeName = tr("Default (%1)").arg(info->displayName);
    info->buildType = type;
    info->buildDirectory = defaultBuildDirectory(projectPath);
    info->workingDirectory = defaultWorkingDirectory(projectPath);
    info->kitId = k->id();
    info->supportsShadowBuild = true; // TODO: is --build-dir a shadow build?

    BBPM_QDEBUG(info->typeName << " in " << projectPath);
    return info;
}

/*static*/ Utils::FileName
BuildConfigurationFactory::defaultBuildDirectory(QString const& top)
{
    return Utils::FileName::fromString(Project::defaultBuildDirectory(top));
}

/*static*/ Utils::FileName
BuildConfigurationFactory::defaultWorkingDirectory(QString const& top)
{
    return Utils::FileName::fromString(Project::defaultWorkingDirectory(top));
}

BuildSettingsWidget::BuildSettingsWidget(BuildConfiguration* bc)
    : bc_(bc)
    , buildPathChooser_(0)
{
    setDisplayName(tr("Boost.Build Manager"));

    QFormLayout* fl = new QFormLayout(this);
    fl->setContentsMargins(0, -1, 0, -1);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    QString const projectPath(bc_->target()->project()->projectDirectory().toString());

    // Working directory
    workPathChooser_ = new Utils::PathChooser(this);
    workPathChooser_->setEnabled(true);
    workPathChooser_->setEnvironment(bc_->environment());
    workPathChooser_->setBaseDirectory(projectPath);
    workPathChooser_->setPath(bc_->workingDirectory().toString());
    fl->addRow(tr("Run Boost.Build in:"), workPathChooser_);

    // Build directory
    buildPathChooser_ = new Utils::PathChooser(this);
    buildPathChooser_->setEnabled(true);
    buildPathChooser_->setEnvironment(bc_->environment());
    buildPathChooser_->setBaseDirectory(projectPath);
    buildPathChooser_->setPath(bc_->rawBuildDirectory().toString());
    fl->addRow(tr("Set --build-dir to:"), buildPathChooser_);

    connect(workPathChooser_, SIGNAL(changed(QString))
          , this, SLOT(workingDirectoryChanged()));

    connect(buildPathChooser_, SIGNAL(changed(QString))
          , this, SLOT(buildDirectoryChanged()));

    connect(bc, SIGNAL(environmentChanged())
          , this, SLOT(environmentHasChanged()));
}

void BuildSettingsWidget::buildDirectoryChanged()
{
    QTC_ASSERT(bc_, return);
    bc_->setBuildDirectory(Utils::FileName::fromString(buildPathChooser_->rawPath()));
}

void BuildSettingsWidget::workingDirectoryChanged()
{
    QTC_ASSERT(bc_, return);
    bc_->setWorkingDirectory(Utils::FileName::fromString(workPathChooser_->rawPath()));
}

void BuildSettingsWidget::environmentHasChanged()
{
    Q_ASSERT(buildPathChooser_);
    buildPathChooser_->setEnvironment(bc_->environment());
}

} // namespace Internal
} // namespace BoostBuildProjectManager
