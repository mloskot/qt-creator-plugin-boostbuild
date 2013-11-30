//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbopenprojectwizard.hpp"
#include "bbproject.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
#include "filesselectionwizardpage.hpp"
// Qt Creator
#include <coreplugin/dialogs/iwizard.h>
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/pathchooser.h>
#include <utils/qtcassert.h>
// Qt
#include <QFileInfo>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace BoostBuildProjectManager {
namespace Internal {

//////////////////////////////////////////////////////////////////////////////////////////

OpenProjectWizard::OpenProjectWizard(Project const* const project)
    : project_(project)
    , projectOpened_(false)
{
    // Project instance has been created, but it's only partially initialised and
    // rest of the initialisation takes place after this wizard completes.
    Q_ASSERT(project_);

    setDisplayName(tr("Open %1 Project").arg(BBPM_C(BOOSTBUILD)));
    setId(BBPM_C(PROJECT_WIZARD_ID));
    setWizardKind(ProjectWizard); // affects dir vs file path and sub-projects handling

    // TODO: do we need categories or flags?
}

bool OpenProjectWizard::run(QString const& platform, QVariantMap const& extraValues)
{
    QVariantMap extraValuesCopy(extraValues);

    // Project name should be passed by caller, but,
    // since we have Project instance handy, double-check.
    if (!extraValuesCopy.contains(BBPM_C(P_KEY_PROJECTNAME)))
        extraValuesCopy.insert(BBPM_C(P_KEY_PROJECTNAME), project_->displayName());

    projectOpened_ = false;
    outputValues_.clear();
    runWizard(project_->projectFilePath(), 0, platform, extraValuesCopy);

    Q_ASSERT(projectOpened_ == !outputValues_.isEmpty());
    return projectOpened_;
}

QWizard*
OpenProjectWizard::createWizardDialog(QWidget* parent
    , Core::WizardDialogParameters const& parameters) const
{

    OpenProjectWizardDialog* wizard(new OpenProjectWizardDialog(parent
        , parameters.defaultPath(), parameters.extraValues()
        , const_cast<OpenProjectWizard*>(this)->outputValues_));

    foreach (QWizardPage* p, parameters.extensionPages())
        BaseFileWizard::applyExtensionPageShortTitle(wizard, wizard->addPage(p));

    return wizard;
}

Core::GeneratedFiles
OpenProjectWizard::generateFiles(QWizard const* wizard, QString* errorMessage) const
{
    Q_UNUSED(errorMessage)

    OpenProjectWizardDialog const* openWizard
        = qobject_cast<OpenProjectWizardDialog const*>(wizard);


    QDir const projectDir(openWizard->path());
    QString const projectName(openWizard->projectName());


    Core::GeneratedFiles files;
    // TODO
    return files;
}

bool
OpenProjectWizard::postGenerateFiles(QWizard const* wizard
    , Core::GeneratedFiles const& files, QString* errorMessage)
{
    Q_UNUSED(wizard);

    projectOpened_
        = ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);

    return projectOpened_;
}

//////////////////////////////////////////////////////////////////////////////////////////
OpenProjectWizardDialog::OpenProjectWizardDialog(QWidget* parent
    , QString const& projectFile
    , QVariantMap const& extraValues, QVariantMap& outputValues)
    : Utils::Wizard(parent)
    , outputValues_(outputValues)
    , extraValues_(extraValues)
    , projectFile_(projectFile)
{
    setWindowTitle(tr("Open %1 Project").arg(BBPM_C(BOOSTBUILD)));

    pathsPage_ = new PathsSelectionWizardPage(this);
    pathsPage_->setTitle(tr("Project Name and Paths"));
    int const pathsPageId = addPage(pathsPage_);
    wizardProgress()->item(pathsPageId)->setTitle(tr("Location"));

    filesPage_ = new FilesSelectionWizardPage(this);
    filesPage_->setTitle(tr("File Selection"));
    int const filesPageId = addPage(filesPage_);
    wizardProgress()->item(filesPageId)->setTitle(tr("Files"));
}


QString OpenProjectWizardDialog::path() const
{
    QFileInfo const projectFileInfo(projectFile());
    QTC_ASSERT(projectFileInfo.isFile(), return QString());

    return projectFileInfo.absoluteDir().absolutePath();
}

QString OpenProjectWizardDialog::projectFile() const
{
    return projectFile_;
}

QString OpenProjectWizardDialog::projectName() const
{
    return pathsPage_->projectName();
}

QString OpenProjectWizardDialog::defaultProjectName() const
{
    return extraValues_.value(BBPM_C(P_KEY_PROJECTNAME)).toString();
}

QStringList OpenProjectWizardDialog::selectedFiles() const
{
    return filesPage_->selectedFiles();
}

QStringList OpenProjectWizardDialog::selectedPaths() const
{
    return filesPage_->selectedPaths();
}

void OpenProjectWizardDialog::setProjectName(QString const& name)
{
    outputValues_.insert(QLatin1String(Constants::P_KEY_PROJECTNAME), name);
}

//////////////////////////////////////////////////////////////////////////////////////////
PathsSelectionWizardPage::PathsSelectionWizardPage(OpenProjectWizardDialog* wizard)
    : QWizardPage(wizard)
    , wizard_(wizard)
{
    QFormLayout *fl = new QFormLayout();
    setLayout(fl);

    QLabel* pathLabel = new QLabel(this);
    pathLabel->setText(tr("Opening the following Jamfile as a project:"));
    fl->addRow(pathLabel);

    QLineEdit* pathLine = new QLineEdit(this);
    pathLine->setReadOnly(true);
    pathLine->setDisabled(true);
    pathLine->setText(wizard_->path());
    fl->addRow(pathLine);

    nameLineEdit_ = new QLineEdit(this);
    nameLineEdit_->setText(wizard_->defaultProjectName());
    fl->addRow(tr("Project name:"), nameLineEdit_);

    QLabel* defaultsLabel = new QLabel(this);
    defaultsLabel->setText(tr("Default Boost.Build runtime locations:"));
    fl->addRow(defaultsLabel);

    QLineEdit* workingLine = new QLineEdit(this);
    workingLine->setReadOnly(true);
    workingLine->setDisabled(true);
    workingLine->setText(Project::defaultWorkingDirectory(wizard_->path()));
    fl->addRow(tr("Working directory:"), workingLine);

    QLineEdit* buildLine = new QLineEdit(this);
    buildLine->setReadOnly(true);
    buildLine->setDisabled(true);
    buildLine->setText(Project::defaultBuildDirectory(wizard_->path()));
    fl->addRow(tr("Build directory:"), buildLine);

    // TODO: indicate if we can find Boost.Build executable?

    QString const info(tr(
        "This allows you to use Qt Creator as an IDE to edit and navigate C++ code, "
        "run %1 command, work through compilation issues, "
        "configure executable targets to run and debug.")
        .arg(QLatin1String(Constants::BOOSTBUILD)));
    QLabel* infoLabel = new QLabel(this);
    infoLabel->setWordWrap(true);
    infoLabel->setText(info);
    fl->addRow(infoLabel);

    connect(nameLineEdit_, &QLineEdit::textChanged
          , wizard_, &OpenProjectWizardDialog::setProjectName);
}

QString PathsSelectionWizardPage::projectName() const
{
    return nameLineEdit_->text();
}

} // namespace Internal
} // namespace BoostBuildProjectManager
