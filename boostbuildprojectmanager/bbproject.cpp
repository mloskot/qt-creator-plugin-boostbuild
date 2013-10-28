#include "bbproject.hpp"
#include "bbprojectfile.hpp"
#include "bbprojectmanager.hpp"
#include "bbprojectmanagerconstants.hpp"
#include <coreplugin/icontext.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <QDir>
#include <QFileInfo>

namespace BoostBuildProjectManager {
namespace Internal {

Project::Project(ProjectManager* manager, QString const& fileName)
    : manager_(manager)
    , fileName_(fileName)
    , projectFile(new ProjectFile(this, fileName))
    , rootNode_(new ProjectNode(this, projectFile_))
{
    Q_ASSERT(manager_);
    Q_ASSERT(!fileName_.isEmpty());

    setId(Constants::PROJECT_ID);
    setProjectContext(Core::Context(Constants::PROJECT_CONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    QFileInfo const fileInfo(fileName_);
    QDir const dir(fileInfo.dir());

    projectName_ = fileInfo.absoluteDir().dirName();
    filesFileName_ = QFileInfo(dir, jamFileName_ + QLatin1String(".files")).absoluteFilePath();

    manager_->registerProject(this);
}

Project::~Project()
{
    manager_->unregisterProject(this);
    delete rootNode_;
}

QString Project::displayName() const
{
    return projectName_;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
