#ifndef BBBUILDINFO_HPP
#define BBBUILDINFO_HPP

#include "bbbuildconfiguration.hpp"
// Qt Creator
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildinfo.h>
#include <utils/fileutils.h>
// Qt

namespace BoostBuildProjectManager {
namespace Internal {

class BuildConfigurationFactory;

class BuildInfo : public ProjectExplorer::BuildInfo
{
public:
    explicit BuildInfo(BuildConfigurationFactory const* f);

    // Boost.Build option variant={debug|release}
    // By default, the debug variant is set.
    BuildConfiguration::BuildType buildType;

    // Boost.Build command working directory.
    // By default, empty what indicates project path from where Jamfile was opened.
    Utils::FileName workingDirectory;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBBUILDINFO_HPP
