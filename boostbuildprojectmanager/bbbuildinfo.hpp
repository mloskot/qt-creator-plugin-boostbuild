#ifndef BBBUILDINFO_HPP_INCLUDED
#define BBBUILDINFO_HPP_INCLUDED

#include "bbbuildconfiguration.hpp"
// Qt Creator
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildinfo.h>
// Qt

namespace BoostBuildProjectManager {
namespace Internal {

class BuildConfigurationFactory;

class BuildInfo : public ProjectExplorer::BuildInfo
{
public:
    explicit BuildInfo(BuildConfigurationFactory const* f);

    // Boost.Build variant={debug|release}
    // By default, the debug variant is set.
    BuildConfiguration::BuildType buildType;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBBUILDINFO_HPP_INCLUDED
