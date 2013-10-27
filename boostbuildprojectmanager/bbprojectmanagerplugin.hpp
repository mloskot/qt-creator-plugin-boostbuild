#ifndef BBPROJECTMANAGERPLUGIN_HPP_INCLUDED
#define BBPROJECTMANAGERPLUGIN_HPP_INCLUDED

#include "bbprojectmanager_global.hpp"

#include <extensionsystem/iplugin.h>

namespace BoostBuildProjectManager {
namespace Internal {

class BoostBuildPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "BoostBuildProjectManager.json")

public:
    BoostBuildPlugin();
    ~BoostBuildPlugin();

    bool initialize(QStringList const& arguments, QString* errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:

};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECTMANAGERPLUGIN_HPP_INCLUDED
