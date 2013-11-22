#include "bbbuildinfo.hpp"
#include "bbbuildconfiguration.hpp"
#include "bbprojectmanagerconstants.hpp"

namespace BoostBuildProjectManager {
namespace Internal {

BuildInfo::BuildInfo(BuildConfigurationFactory const* f)
    : ProjectExplorer::BuildInfo(f)
    , buildType(BuildConfiguration::Debug)
{
}

} // namespace Internal
} // namespace BoostBuildProjectManager
