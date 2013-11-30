//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbopenprojectwizard.hpp"
#include "bbproject.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <coreplugin/dialogs/iwizard.h>
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/pathchooser.h>
// Qt
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

namespace BoostBuildProjectManager {
namespace Internal {

//////////////////////////////////////////////////////////////////////////////////////////

OpenProjectWizard::OpenProjectWizard()
    : projectOpened_(false)
{
}

bool
OpenProjectWizard::run(QString const& path, QString const& platform
    , QVariantMap const& extraValues)
{
    projectOpened_ = false;
    outputValues_.clear();
    runWizard(path, 0, platform, extraValues);

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

    OpenProjectWizardDialog const* opw
        = qobject_cast<OpenProjectWizardDialog const*>(wizard);

    opw->windowTitle();

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
OpenProjectWizardDialog::OpenProjectWizardDialog(QWidget* parent, QString const& path
    , QVariantMap const& extraValues, QVariantMap& outputValues)
    : Utils::Wizard(parent)
    , outputValues_(outputValues)
    , extraValues_(extraValues)
    , path_(path)
{
    setWindowTitle(tr("Open %1 Project").arg(QLatin1String(Constants::BOOSTBUILD)));

    pathsPage_ = new PathsSelectionWizardPage(this);
    pathsPage_->setTitle(tr("Project Name and Paths"));
    int const pathsPageId = addPage(pathsPage_);
    wizardProgress()->item(pathsPageId)->setTitle(tr("Location"));

    //int const filesPageId = addPage(filesPage_);
    //wizardProgress()->item(filesPageId)->setTitle(tr("Files"));
}

QString OpenProjectWizardDialog::projectName() const
{
    return pathsPage_->projectName();
}

QString OpenProjectWizardDialog::defaultProjectName() const
{
    return extraValues_.value(BBPM_C(P_KEY_PROJECTNAME)).toString();
}

QString OpenProjectWizardDialog::projectPath() const
{
    return path_;
}

QStringList OpenProjectWizardDialog::selectedFiles() const
{
    return QStringList();
}

QStringList OpenProjectWizardDialog::selectedPaths() const
{
    return QStringList();
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
    pathLine->setText(wizard_->projectPath());
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
    workingLine->setText(Project::defaultWorkingDirectory(wizard_->projectPath()));
    fl->addRow(tr("Working directory:"), workingLine);

    QLineEdit* buildLine = new QLineEdit(this);
    buildLine->setReadOnly(true);
    buildLine->setDisabled(true);
    buildLine->setText(Project::defaultBuildDirectory(wizard_->projectPath()));
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

//////////////////////////////////////////////////////////////////////////////////////////
#if 0
FilesSelectionWizardPage::FilesSelectionWizardPage(
    OpenProjectWizard* genericProjectWizard
  , QWidget* parent)
{
}

bool FilesSelectionWizardPage::isComplete() const
{
}

void FilesSelectionWizardPage::initializePage()
{
}

void FilesSelectionWizardPage::cleanupPage()
{
}

QStringList FilesSelectionWizardPage::selectedFiles() const
{

}

QStringList FilesSelectionWizardPage::selectedPaths() const
{
}

void FilesSelectionWizardPage::applyFilter()
{
}

void FilesSelectionWizardPage::parsingProgress(const QString &text)
{
}

void FilesSelectionWizardPage::parsingFinished()
{
}

void FilesSelectionWizardPage::createHideFileFilterControls(QVBoxLayout* layout)
{

}

void FilesSelectionWizardPage::createShowFileFilterControls(QVBoxLayout* layout)
{

}

void FilesSelectionWizardPage::createApplyButton(QVBoxLayout *layout)
{

}
#endif

} // namespace Internal
} // namespace BoostBuildProjectManager
