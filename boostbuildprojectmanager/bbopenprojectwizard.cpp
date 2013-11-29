//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbopenprojectwizard.hpp"
#include "bbproject.hpp"
#include "bbprojectmanagerconstants.hpp"
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
OpenProjectWizard::OpenProjectWizard(QString const& name, QString const& path, QWidget* parent)
    : Utils::Wizard(parent)
    , name_(name)
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

QString OpenProjectWizard::projectName() const
{
    return pathsPage_->projectName();
}

QString OpenProjectWizard::defaultProjectName() const
{
    return name_;
}

QString OpenProjectWizard::projectPath() const
{
    return path_;
}

QStringList OpenProjectWizard::selectedFiles() const
{
    return QStringList();
}

QStringList OpenProjectWizard::selectedPaths() const
{
    return QStringList();
}

//////////////////////////////////////////////////////////////////////////////////////////
PathsSelectionWizardPage::PathsSelectionWizardPage(OpenProjectWizard* wizard)
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
    workingLine->setText(Project::defaultWorkingDirectory(wizard_->projectPath()));
    fl->addRow(tr("Working directory:"), workingLine);

    QLineEdit* buildLine = new QLineEdit(this);
    buildLine->setReadOnly(true);
    buildLine->setText(Project::defaultBuildDirectory(wizard_->projectPath()));
    fl->addRow(tr("Build directory:"), buildLine);

    // TODO: find Boost.Build executable?

    QString const info(tr(
        "This allows you to use Qt Creator as an IDE to edit and navigate C++ code, "
        "run %1 command, work through compilation issues, "
        "configure executable targets to run and debug.")
        .arg(QLatin1String(Constants::BOOSTBUILD)));
    QLabel* infoLabel = new QLabel(this);
    infoLabel->setWordWrap(true);
    infoLabel->setText(info);
    fl->addRow(infoLabel);
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