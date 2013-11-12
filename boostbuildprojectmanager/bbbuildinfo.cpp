#include "bbbuildinfo.hpp"
#include "bbbuildconfiguration.hpp"
#include "bbprojectmanagerconstants.hpp"

namespace BoostBuildProjectManager {
namespace Internal {

BuildInfo::BuildInfo(BuildConfigurationFactory const* f)
    : ProjectExplorer::BuildInfo(f)
    , buildVariant(BuildConfiguration::Debug)
{
    setBuildVariant(buildVariant);
}

void BuildInfo::setBuildVariant(BuildConfiguration::BuildType variant)
{
    buildVariant = variant;
    displayName = QLatin1String(variant == BuildConfiguration::Debug
                    ? Constants::VARIANT_DEBUG
                    : Constants::VARIANT_RELEASE);
}

} // namespace Internal
} // namespace BoostBuildProjectManager
