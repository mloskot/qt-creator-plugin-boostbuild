#ifndef BBPROJECTMANAGERCONSTANTS_HPP_INCLUDED
#define BBPROJECTMANAGERCONSTANTS_HPP_INCLUDED

#include <qglobal.h>

namespace BoostBuildProjectManager {
namespace Constants {

char const BOOSTBUILD[] = "Boost.Build";

char const PROJECT_CONTEXT[] = "BoostBuildProjectManager.ProjectContext";
char const PROJECT_ID[]      = "BoostBuildProjectManager.Project";

char const BUILDCONFIGURATION_ID[] = "BoostBuildProjectManager.BuildConfiguration";
char const BUILDSTEP_ID[] = "BoostBuildProjectManager.BuildStep";

char const TASK_PROJECT_READING[] = "BoostBuildProjectManager.Task.ProjectReading";

char const MIMETYPE_PROJECT[] = "text/x-boostbuild-project";
char const MIMETYPE_JAMFILE[] = "application/vnd.boostbuild.v2";
char const MIMETYPE_JAMFILE_FILES[]   = "application/vnd.qtcreator.boostbuild.files";
char const JAMFILE_FILES_EXT[] = ".qtcreator.files";
char const JAMFILE_INCLUDES_EXT[] = ".qtcreator.includes";

char const OUTPUT_PARSER[] = "Boost.Build Parser";

char const COMMAND_BB2[] = "b2";
char const COMMAND_BJAM[] = "bjam";

char const VARIANT_DEBUG[] = QT_TR_NOOP("Debug");
char const VARIANT_RELEASE[] = QT_TR_NOOP("Release");

// Set meaningful names for common boolean values
bool const FileNotGenerated = false; // used with GeneratedFile
bool const ReturnValueNotIgnored = false; // used with BuildStep

} // namespace Constants
} // namespace BoostBuildProjectManager

#endif // BBPROJECTMANAGERCONSTANTS_HPP_INCLUDED
