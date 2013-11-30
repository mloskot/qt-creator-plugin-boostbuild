//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#ifndef BBOPENPROJECTWIZARD_HPP
#define BBOPENPROJECTWIZARD_HPP

// Qt Creator
#include <coreplugin/basefilewizard.h>
#include <coreplugin/generatedfile.h>
#include <utils/wizard.h>
// Qt
#include <QString>
#include <QStringList>
QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QLabel;
class QTreeView;
class QLineEdit;
QT_END_NAMESPACE

namespace Utils {
class PathChooser;
}

namespace BoostBuildProjectManager {
namespace Internal {

class Project;
class PathsSelectionWizardPage;
class FilesSelectionWizardPage;

//////////////////////////////////////////////////////////////////////////////////////////
// NOTE: The Boost.Build wizard is based on Core::BaseFileWizard which seems to be
// dedicated to build "New Project" wizards. So, the plugin uses the base class in
// unconventional, matching its features to Boost.Build wizard needs, like:
// - no parent QWidget is used
// - platform name is set from default Kit display name, usually it's "Desktop"
// - extra values QVariantMap may carry custom data
// CAUTION: This wizard may stop building or start failing in run-time,
// if Qt Creator changes the base class significantly.
class OpenProjectWizard : public Core::BaseFileWizard
{
    Q_OBJECT

public:
    OpenProjectWizard();

    bool
    run(QString const& path, QString const& platform, QVariantMap const& extraValues);

    QVariantMap outputValues() const { return outputValues_; }

protected:

    QWizard*
    createWizardDialog(QWidget* parent
        , Core::WizardDialogParameters const& parameters) const;

    Core::GeneratedFiles
    generateFiles(QWizard const* baseWizard, QString* errorMessage) const;

    bool
    postGenerateFiles(QWizard const* wizard
        , Core::GeneratedFiles const& files, QString* errorMessage);

private:
    QVariantMap outputValues_;
    bool projectOpened_;
};

//////////////////////////////////////////////////////////////////////////////////////////
class OpenProjectWizardDialog : public Utils::Wizard
{
    Q_OBJECT

public:
    OpenProjectWizardDialog(QWidget* parent, QString const& path
        , QVariantMap const& extraValues, QVariantMap& outputValues);

    QString projectName() const;
    QString defaultProjectName() const;
    QString projectPath() const;

    QStringList selectedFiles() const;
    QStringList selectedPaths() const;

public slots:
    void setProjectName(QString const& name);

private:
    QVariantMap& outputValues_;
    QVariantMap extraValues_;
    QString path_;
    PathsSelectionWizardPage* pathsPage_;
    FilesSelectionWizardPage* filesPage_;
};

//////////////////////////////////////////////////////////////////////////////////////////
class PathsSelectionWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PathsSelectionWizardPage(OpenProjectWizardDialog* wizard);

    QString projectName() const;
    void setProjectName(QString const& name);

private:
    OpenProjectWizardDialog* wizard_;
    QLineEdit* nameLineEdit_;
};

#if 0
//////////////////////////////////////////////////////////////////////////////////////////
class FilesSelectionWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit FilesSelectionWizardPage(OpenProjectWizard* wizard);

    bool isComplete() const;
    void initializePage();
    void cleanupPage();
    QStringList selectedFiles() const;
    QStringList selectedPaths() const;

private slots:
    void applyFilter();
    void parsingProgress(const QString &text);
    void parsingFinished();

private:
    void createHideFileFilterControls(QVBoxLayout* layout);
    void createShowFileFilterControls(QVBoxLayout* layout);
    void createApplyButton(QVBoxLayout *layout);

    OpenProjectWizardDialog* openProjectWizardDialog_;
    //TODO: SelectableFilesModel* model_;

    QLabel* hideFilesFilterLabel_;
    QLineEdit* hideFilesfilterLineEdit_;

    QLabel* showFilesFilterLabel_;
    QLineEdit* showFilesfilterLineEdit_;

    QPushButton* applyFilterButton_;

    QTreeView* view_;
    QLabel* label_;
    bool finished_;
};
#endif

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBOPENPROJECTWIZARD_HPP
