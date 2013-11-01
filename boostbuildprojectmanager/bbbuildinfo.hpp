#ifndef BBBUILDINFO_HPP_INCLUDED
#define BBBUILDINFO_HPP_INCLUDED

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
    explicit BuildInfo(BuildConfigurationFactory* f);

    ProjectExplorer::BuildConfiguration::BuildType type;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBBUILDINFO_HPP_INCLUDED
