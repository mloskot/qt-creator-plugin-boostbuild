#ifndef BBPROJECTMANAGERPLUGIN_HPP
#define BBPROJECTMANAGERPLUGIN_HPP

#include "bbprojectmanager_global.hpp"

#include <extensionsystem/iplugin.h>

namespace BoostBuildProjectManager {
namespace Internal {


// Sole implementation of the IPlugin class for the extension.
class BoostBuildPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "BoostBuildProjectManager.json")

public:
    BoostBuildPlugin();
    ~BoostBuildPlugin();

    // Called as second step of the plugins loading.
    // The initialize functions are called in root-to-leaf order of the dependency tree.
    bool initialize(QStringList const& arguments, QString* errorString);

    // Called as final step of the plugins loading.
    // At this point, all dependencies along the plugin dependency tree
    // have been initialized completely.
    void extensionsInitialized();

    ShutdownFlag aboutToShutdown();
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECTMANAGERPLUGIN_HPP
