#ifndef BBBUILDSTEP_HPP_INCLUDED
#define BBBUILDSTEP_HPP_INCLUDED

// Qt Creator
#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/task.h>
// Qt
#include <QLineEdit>
#include <QList>
#include <QString>
#include <QVariantMap>

namespace ProjectExplorer {
class Project;
}

namespace BoostBuildProjectManager {
namespace Internal {

// TODO: see doc in AutogenStep
class BuildStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT
    friend class BuildStepFactory;
    friend class BuildStepConfigWidget;

public:

    BuildStep(ProjectExplorer::BuildStepList* bsl);

    bool init();
    void run(QFutureInterface<bool>& interface);
    ProjectExplorer::BuildStepConfigWidget* createConfigWidget();
    bool immutable() const;
    QString additionalArguments() const;
    QVariantMap toMap() const;

    enum StepType { Build, Clean };

    void setStepType(StepType type);

protected:
    BuildStep(ProjectExplorer::BuildStepList* bsl, BuildStep* bs);
    BuildStep(ProjectExplorer::BuildStepList* bsl, const Core::Id id);

    bool fromMap(QVariantMap const& map);

private:
    QList<ProjectExplorer::Task> m_tasks;
    StepType m_stepType;
};

// Factory used to create instances of BuildStep.
class BuildStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT

public:
    BuildStepFactory(QObject* parent = 0);

    QList<Core::Id> availableCreationIds(ProjectExplorer::BuildStepList* bc) const;
    QString displayNameForId(const Core::Id id) const;

    bool canCreate(ProjectExplorer::BuildStepList* parent, Core::Id const id) const;
    ProjectExplorer::BuildStep*
    create(ProjectExplorer::BuildStepList* parent, Core::Id const id);

    bool canClone(ProjectExplorer::BuildStepList *parent
                , ProjectExplorer::BuildStep *source) const;
    ProjectExplorer::BuildStep*
    clone(ProjectExplorer::BuildStepList* parent, ProjectExplorer::BuildStep* source);

    bool canRestore(ProjectExplorer::BuildStepList* parent, QVariantMap const& map) const;
    ProjectExplorer::BuildStep*
    restore(ProjectExplorer::BuildStepList* parent, QVariantMap const& map);

    bool canHandle(ProjectExplorer::BuildStepList* parent) const;
};

class BuildStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    BuildStepConfigWidget(BuildStep* buildStep);
    ~BuildStepConfigWidget();
    QString displayName() const;
    QString summaryText() const;

private slots:
    // TODO:
    //void itemChanged(QListWidgetItem* item);
    //void makeLineEditTextEdited();
    //void makeArgumentsLineEditTextEdited();
    //void updateMakeOverrrideLabel();
    void updateDetails();

private:
    //Ui::GenericMakeStep* m_ui;
    BuildStep* step_;
    QString summary_;
    QLineEdit* arguments_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBBUILDSTEP_HPP_INCLUDED

