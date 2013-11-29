//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbbuildconfiguration.hpp"
#include "bbbuildstep.hpp"
#include "bbopenprojectwizard.hpp"
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
#include <coreplugin/mimedatabase.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <cpptools/cppmodelmanagerinterface.h>
#include <cpptools/cpptoolsconstants.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitinformation.h>
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

Project::Project(ProjectManager* manager, QString const& fileName)
    : manager_(manager)
    , filePath_(fileName)
    , projectFile_(new ProjectFile(this, filePath_)) // enables projectDirectory()
    , projectNode_(new ProjectNode(this, projectFile_))
    , projectReader_(projectDirectory(projectFile_->filePath())) // avoid virtual call
{
    Q_ASSERT(manager_);
    Q_ASSERT(!filePath_.isEmpty());

    setProjectContext(Core::Context(Constants::PROJECT_CONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    QFileInfo const projectFileInfo(filePath_);
    QDir const projectDir(projectFileInfo.dir());
    projectName_ = defaultProjectName(filePath_);
    filesFilePath_ = QFileInfo(projectDir
        , filePath_ + QLatin1String(Constants::JAMFILE_FILES_EXT)).absoluteFilePath();
    includesFilePath_ = QFileInfo(projectDir
        , filePath_ + QLatin1String(Constants::JAMFILE_INCLUDES_EXT)).absoluteFilePath();

    projectNode_->setDisplayName(displayName());

    manager_->registerProject(this);

    // TODO
    //projectFileWatcher_->addPath(projectFilePath);
    //connect(projectFileWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(refresh()));

    BBPM_QDEBUG("created project: " << displayName() << " in " << projectDirectory());
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
    // TODO: handle ExcludeGeneratedFiles, but what files exactly?
    //       *.qtcreator.files, *.qtcreator.includes and *.user?
    Q_UNUSED(fileMode);
    return files_;
}

QStringList Project::files() const
{
    return files(FilesMode::AllFiles);
}

QString Project::filesFilePath() const
{
    Q_ASSERT(!filesFilePath_.isEmpty());
    return filesFilePath_;
}

QString Project::includesFilePath() const
{
    Q_ASSERT(!includesFilePath_.isEmpty());
    return includesFilePath_;
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

/*static*/
QString Project::defaultProjectName(QString const& fileName)
{
    QFileInfo const fileInfo(fileName);
    return fileInfo.absoluteDir().dirName();
}

/*static*/
QString Project::defaultBuildDirectory(QString const& top)
{
    Utils::FileName fn(Utils::FileName::fromString(defaultWorkingDirectory(top)));
    fn.appendPath(QLatin1String(Constants::BUILD_DIR_NAME));
    return fn.toString();
}

/*static*/
QString Project::defaultWorkingDirectory(QString const& top)
{
    // Accepts both, project file or project directory, as top.
    return ProjectExplorer::Project::projectDirectory(top);
}

QVariantMap Project::toMap() const
{
    QVariantMap map(ProjectExplorer::Project::toMap());
    map.insert(QLatin1String(Constants::P_KEY_PROJECTNAME), projectName_);
    return map;
}

// This function is called at the very beginning to restore the settings
// from .user file, if there is any with previous settings stored.
bool Project::fromMap(QVariantMap const& map)
{
    BBPM_QDEBUG(displayName());
    QTC_ASSERT(projectNode_, return false);

    if (!ProjectExplorer::Project::fromMap(map))
        return false;

    // TODO: do we need on-loading wizard similar to CMakeProjectManager?

    // Set up active ProjectConfiguration (aka Target).
    // NOTE: Call setActiveBuildConfiguration when creating new build configurations.

    if (!activeTarget())
    {
        // Create project configuration from scratch

        OpenProjectWizard opw(displayName(), projectFilePath());
        if (opw.exec() != QDialog::Accepted)
            return false;
        projectName_ = opw.projectName();

        // TODO: Map the Kit to Boost.Build toolset option value
        ProjectExplorer::Kit* defaultKit = ProjectExplorer::KitManager::defaultKit();
        Q_ASSERT(defaultKit);

        // Creates as many {Build|Run|Deploy}Configurations for as corresponding
        // factories report as available.
        // For example, BuildConfigurationFactory::availableBuilds => Debug and Release
        ProjectExplorer::Target* target = createTarget(defaultKit);
        QTC_ASSERT(target, return false);

        addTarget(target);
    }
    else
    {
        // Configure project from settings sorced from .user file
        projectName_ = map.value(QLatin1String(Constants::P_KEY_PROJECTNAME)).toString();

        BBPM_QDEBUG(displayName() << "has user file");
    }

    // TODO: create projectNameChanged signal
    projectNode_->setDisplayName(displayName());

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

    // Trigger loading project tree and parsing sources
    refresh();

    return true;
}

void Project::refresh()
{
    if (QFileInfo(filesFilePath()).exists())
    {
        QSet<QString> oldFileList;
        oldFileList = files_.toSet();

        // Parse project:
        // The manager does not parse Jamfile files.
        // Only generates and parses list of source files in Jamfile.${JAMFILE_FILES_EXT}
        QString const projectPath(projectDirectory());
        filesRaw_ = Utility::readLines(filesFilePath());
        files_ = Utility::makeAbsolutePaths(projectPath, filesRaw_);

        QStringList includePaths =
            Utility::makeAbsolutePaths(projectPath,
                Utility::readLines(includesFilePath()));

        emit fileListChanged();

        projectNode_->refresh(oldFileList);

        // TODO: Does it make sense to move this to separate asynchronous task?
        // TODO: extract updateCppCodeModel
        using CppTools::CppModelManagerInterface;
        if (CppModelManagerInterface* cppModel = CppModelManagerInterface::instance())
        {
            CppModelManagerInterface::ProjectInfo cppInfo = cppModel->projectInfo(this);
            cppInfo.clearProjectParts();

            CppTools::ProjectPart::Ptr cppPart(new CppTools::ProjectPart());
            cppPart->project = this;
            cppPart->displayName = displayName();
            cppPart->projectFile = projectFilePath();
            cppPart->includePaths += projectDirectory();
            cppPart->includePaths += includePaths;

            cppPart->cxxVersion = CppTools::ProjectPart::CXX11;
            // TODO: waiting for Jamfile parser
            //cppPart->defines +=

            ProjectExplorer::Kit const* k = activeTarget()
                ? activeTarget()->kit() : ProjectExplorer::KitManager::defaultKit();
            if (ProjectExplorer::ToolChain* tc
                    = ProjectExplorer::ToolChainKitInformation::toolChain(k))
            {
                // TODO: form GenericProjectmanager: FIXME: Can we do better?
                QStringList cxxflags;
                cppPart->evaluateToolchain(
                    tc, cxxflags, cxxflags
                  , ProjectExplorer::SysRootKitInformation::sysRoot(k));
            }

            CppTools::ProjectFileAdder adder(cppPart->files);
            foreach (QString const& file, files_)
                adder.maybeAdd(file);

            cppModelFuture_.cancel();

            cppInfo.appendProjectPart(cppPart);

            cppModel ->updateProjectInfo(cppInfo);
            bool const cppEnabled = !cppPart->files.isEmpty();
            setProjectLanguage(ProjectExplorer::Constants::LANG_CXX, cppEnabled);

            cppModelFuture_ = cppModel->updateProjectInfo(cppInfo);
        }
    }
    else
    {
        // Read project tree on filesystem.

        connect(&projectReader_, SIGNAL(readingFinished())
              , this, SLOT(handleReadingFinished()));

        projectReader_.startReading();
    }
}

void Project::handleReadingFinished()
{
    QDir const projectDir(projectDirectory());
    QString errorMessage;

    // Generate .files file, all paths should be relative
    QStringList sources = projectReader_.files();
    Utility::makeRelativePaths(projectDir.absolutePath(), sources);

    Core::GeneratedFile gfFilesFile(filesFilePath());
    gfFilesFile.setContents(sources.join(QLatin1String("\n")));
    if (!gfFilesFile.write(&errorMessage))
    {
        QMessageBox::critical(0, tr("File Generation Failure"), errorMessage);
        return;
    }

    // Generate .includes file with all directories with headers
    QStringList const includePaths = projectReader_.includePaths();
    Core::GeneratedFile gfIncludesFile(includesFilePath());
    gfIncludesFile.setContents(includePaths.join(QLatin1String("\n")));
    if (!gfIncludesFile.write(&errorMessage))
    {
        QMessageBox::critical(0, tr("File Generation Failure"), errorMessage);
        return;
    }

    // Auxiliary files ready, complete refreshing project
    refresh();
}

} // namespace Internal
} // namespace BoostBuildProjectManager
