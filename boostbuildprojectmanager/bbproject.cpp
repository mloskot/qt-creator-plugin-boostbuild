#include "bbproject.hpp"
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
{
    Q_ASSERT(manager_);

    setId(Constants::PROJECT_ID);
    setProjectContext(Core::Context(Constants::PROJECT_CONTEXT));
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::LANG_CXX));

    QFileInfo const fileInfo(fileName_);
    QDir const dir(fileInfo.dir());

    projectName_ = fileInfo.absoluteDir().dirName();
    jamFileName_ = fileInfo.absoluteFilePath();
    filesFileName_ = QFileInfo(dir, jamFileName_ + QLatin1String(".files")).absoluteFilePath();

    manager_->registerProject(this);
}

} // namespace Internal
} // namespace BoostBuildProjectManager
