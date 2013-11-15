//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbbuildconfiguration.hpp"
#include "bbbuildstep.hpp"
#include "bbproject.hpp"
#include "bbprojectfile.hpp"
#include "bbprojectmanager.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbprojectnode.hpp"
#include "bbprojectreader.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/generatedfile.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/target.h>
#include <qtsupport/customexecutablerunconfiguration.h>
#include <utils/QtConcurrentTools>
// Qt
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

namespace BoostBuildProjectManager {
namespace Internal {

// Project /////////////////////////////////////////////////////////////////////
Project::Project(ProjectManager* manager, QString const& fileName)
    : manager_(manager)
    , fileName_(fileName)
    , projectFile_(new ProjectFile(this, fileName_)) // enables projectDirectory()
    , projectNode_(new ProjectNode(this, projectFile_))
    , projectReader_(projectDirectory(projectFile_->filePath())) // avoid virtual call
{
    Q_ASSERT(manager_);
    Q_ASSERT(!fileName_.isEmpty());

    setProjectContext(Core::Context(Constants::PROJECT_CONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    QFileInfo const fileInfo(fileName_);
    QDir const dir(fileInfo.dir());
    QString const filesFileName(fileName_ + QLatin1String(Constants::JAMFILE_FILES_EXT));

    projectName_ = fileInfo.absoluteDir().dirName();
    filesFileName_ = QFileInfo(filesFileName).absoluteFilePath();

    projectNode_->setDisplayName(projectName_);

    manager_->registerProject(this);

    BBPM_QDEBUG("created project: "
                << displayName()
                << " (" << fileInfo.completeBaseName() << ")"
                << " in " << projectDirectory());
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
    BBPM_QDEBUG(displayName() << "has" << files_.size() << "files");
    //TODO: return projectReader_.files();
    return files_;
}

QStringList Project::files() const
{
    return files(FilesMode::AllFiles);
}

QString Project::filesFileName() const
{
    return filesFileName_;
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

void Project::refresh()
{
    QSet<QString> oldFileList;
    oldFileList = files_.toSet();

    // Parse project:
    // The manager does not parse Jamfile files.
    // Only generates and parses list of source files in Jamfile.${JAMFILE_FILES_EXT}
    QString const projectPath(projectDirectory());
    filesRaw_ = Utility::readLines(filesFileName());
    files_ = Utility::makeAbsolutePaths(projectPath, filesRaw_);

    emit fileListChanged();

    projectNode_->refresh(oldFileList);
}

// This function is called at the very beginning to restore the settings
// from .user file, if there is any with previous settings stored.
bool Project::fromMap(QVariantMap const& map)
{
    BBPM_QDEBUG(displayName());

    if (!ProjectExplorer::Project::fromMap(map))
        return false;

    // Set up reading of project tree file list.
    connect(&projectReader_, SIGNAL(readingFinished())
           ,this, SLOT(handleReadingFinished()));

    // TODO: If .files file exists, do not generate but read it.
    projectReader_.startReading();

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

    QTC_ASSERT(hasActiveBuildSettings(), return false);
    QTC_ASSERT(activeTarget() != 0, return false);

    return true;
}

void Project::handleReadingFinished()
{
    BBPM_QDEBUG("signalled");

    // Write .files file
    QStringList const sources = projectReader_.files();
    Core::GeneratedFile generatedFilesFile(filesFileName_);
    generatedFilesFile.setContents(sources.join(QLatin1String("\n")));

    QString errorMessage;
    if (!generatedFilesFile.write(&errorMessage))
    {
        QMessageBox::critical(0, tr("File Generation Failure"), errorMessage);
        return;
    }

    refresh();
}

// ProjectFilesFile ////////////////////////////////////////////////////////////
ProjectFilesFile::ProjectFilesFile(Project* parent, QString const& fileName)
    : Core::IDocument(parent)
    , project_(parent)
{
    setFilePath(fileName);
}

bool ProjectFilesFile::save(QString* errorString, QString const& fileName, bool autoSave)
{
    Q_UNUSED(errorString)
    Q_UNUSED(fileName)
    Q_UNUSED(autoSave)
    return false;
}

QString ProjectFilesFile::defaultPath() const
{
    return QString();
}

QString ProjectFilesFile::suggestedFileName() const
{
    return QString();
}

QString ProjectFilesFile::mimeType() const
{
    return QLatin1String(Constants::MIMETYPE_JAMFILE_FILES);
}

bool ProjectFilesFile::isModified() const
{
    return false;
}

bool ProjectFilesFile::isSaveAsAllowed() const
{
    return false;
}


Core::IDocument::ReloadBehavior
ProjectFilesFile::reloadBehavior(Core::IDocument::ChangeTrigger state
                               , Core::IDocument::ChangeType type) const
{
    Q_UNUSED(state)
    Q_UNUSED(type)
    return Core::IDocument::BehaviorSilent;
}

bool ProjectFilesFile::reload(QString* errorString, ReloadFlag flag, ChangeType type)
{
    Q_UNUSED(errorString)
    Q_UNUSED(flag)

    if (type == Core::IDocument::TypePermissions)
        return true;

    // TODO: reload list of files

    return true;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
