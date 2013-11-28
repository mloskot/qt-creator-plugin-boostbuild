//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbopenprojectwizard.hpp"
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
#include <QVBoxLayout>

namespace BoostBuildProjectManager {
namespace Internal {

//////////////////////////////////////////////////////////////////////////////////////////
OpenProjectWizard::OpenProjectWizard(QWidget* parent)
    : Utils::Wizard(parent)
{
    setWindowTitle(tr("Open %1 Project").arg(Constants::BOOSTBUILD));

}

QString OpenProjectWizard::path() const
{

}

void OpenProjectWizard::setPath(QString const& path)
{

}

QStringList OpenProjectWizard::selectedFiles() const
{

}

QStringList OpenProjectWizard::selectedPaths() const
{

}

QString OpenProjectWizard::projectName() const
{
}

//////////////////////////////////////////////////////////////////////////////////////////
PathsSelectionWizardPage::PathsSelectionWizardPage(OpenProjectWizard* wizard)
    : QWizardPage(wizard)
    , wizard(wizard_)
{
    setTitle(tr("Open %1 Project").arg(Constants::BOOSTBUILD));
    QString const description(
        tr("Opens Jamroot or Jamfile as a project. "
           "This allows you to use Qt Creator as an IDE to edit code, "
           "configure run targets to be executed and debugged."
           "run %1 command, navigate through compilation issues and ")
        .arg(Constants::BOOSTBUILD));)

    QFormLayout *fl = new QFormLayout();
    setLayout(fl);

    QLabel* descriptionLabel = new QLabel(this);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setText(description);
    fl->addRow(descriptionLabel);

    workPathChooser_ = new Utils::PathChooser(this);
    workPathChooser_->setExpectedKind(Utils::PathChooser::Directory);
    workPathChooser_->setBaseDirectory(wizard_->projectDirectory());
    workPathChooser_->setPath(workPathChooser_->baseDirectory());
    fl->addRow(workPathChooser_);

    buildPathChooser_ = new Utils::PathChooser(this);
    buildPathChooser_->setExpectedKind(Utils::PathChooser::Directory);
    buildPathChooser_->setBaseDirectory(wizard_->buildDirectory());
    buildPathChooser_->setPath(buildPathChooser_->baseDirectory());
    fl->addRow(buildPathChooser_);

    // TODO: signals
}

//////////////////////////////////////////////////////////////////////////////////////////
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

} // namespace Internal
} // namespace BoostBuildProjectManager
