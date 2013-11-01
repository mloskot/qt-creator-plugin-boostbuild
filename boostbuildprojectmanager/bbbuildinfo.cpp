#include "bbbuildinfo.hpp"
#include "bbbuildconfiguration.hpp"

namespace BoostBuildProjectManager {
namespace Internal {

BuildInfo::BuildInfo(BuildConfigurationFactory* f)
    : ProjectExplorer::BuildInfo(f)
{
}

} // namespace Internal
} // namespace BoostBuildProjectManager
