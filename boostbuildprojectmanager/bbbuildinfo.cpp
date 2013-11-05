#include "bbbuildinfo.hpp"
#include "bbbuildconfiguration.hpp"

namespace BoostBuildProjectManager {
namespace Internal {

BuildInfo::BuildInfo(BuildConfigurationFactory const* f)
    : ProjectExplorer::BuildInfo(f)
{
}

} // namespace Internal
} // namespace BoostBuildProjectManager
