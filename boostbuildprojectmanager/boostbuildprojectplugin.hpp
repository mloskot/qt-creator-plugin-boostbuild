#ifndef BOOSTBUILDPLUGIN_HPP
#define BOOSTBUILDPLUGIN_HPP

#include "boostbuild_global.hpp"

#include <extensionsystem/iplugin.h>

namespace BoostBuild { namespace Internal {

class BoostBuildPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "BoostBuild.json")

public:
    BoostBuildPlugin();
    ~BoostBuildPlugin();

    bool initialize(QStringList const& arguments, QString* errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void triggerAction();
};

}}

#endif // BOOSTBUILDPLUGIN_HPP

