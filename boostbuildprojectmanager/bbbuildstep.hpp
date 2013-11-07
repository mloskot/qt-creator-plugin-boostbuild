#ifndef BBBUILDSTEP_HPP_INCLUDED
#define BBBUILDSTEP_HPP_INCLUDED

// Qt Creator
#include <projectexplorer/abstractprocessstep.h>
// Qt

namespace ProjectExplorer {
class BuildStep;
class IBuildStepFactory;
class Project;
}

namespace BoostBuildProjectManager {
namespace Internal {

class BuildStep : public ProjectExplorer::AbstractBuildStep
{
    Q_OBJECT

public:
    BuildStep(ProjectExplorer::BuildStepList* bsl);

    bool init();
    void run(QFutureInterface<bool>& interface);
    ProjectExplorer::BuildStepConfigWidget* createConfigWidget();
    bool immutable() const;
    QString additionalArguments() const;
    QVariantMap toMap() const;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBBUILDSTEP_HPP_INCLUDED

