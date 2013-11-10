#ifndef BBPROJECTMANAGERCONSTANTS_HPP_INCLUDED
#define BBPROJECTMANAGERCONSTANTS_HPP_INCLUDED

namespace BoostBuildProjectManager {
namespace Constants {

char const BOOSTBUILD[] = "Boost.Build";

char const PROJECT_CONTEXT[] = "BoostBuildProjectManager.ProjectContext";
char const PROJECT_ID[]      = "BoostBuildProjectManager.Project";

char const BUILDCONFIGURATION_ID[] = "BoostBuildProjectManager.BuildConfiguration";
char const BUILDSTEP_ID[] = "BoostBuildProjectManager.BuildStep";

char const BJAM_COMMAND[] = "bjam";
char const BB2_COMMAND[] = "b2";

char const MIMETYPE_PROJECT[] = "text/x-boostbuild-project";
char const MIMETYPE_JAMFILE[] = "application/vnd.boostbuild.v2";
char const MIMETYPE_FILES[]   = "application/vnd.qtcreator.boostbuild.files";

} // namespace Constants
} // namespace BoostBuildProjectManager

#endif // BBPROJECTMANAGERCONSTANTS_HPP_INCLUDED
