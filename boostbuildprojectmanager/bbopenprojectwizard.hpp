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

// TODO: Make the wizard use Core::BaseFileWizard features with Summary
//       page at the end based on ProjectWizardPage

class Project;
class PathsSelectionWizardPage;
class FilesSelectionWizardPage;

class OpenProjectWizard : public Utils::Wizard
{
    Q_OBJECT

public:
    OpenProjectWizard(QString const& name, QString const& path, QWidget* parent = 0);

    QString projectName() const;
    QString defaultProjectName() const;
    QString projectPath() const;

    QStringList selectedFiles() const;
    QStringList selectedPaths() const;

private:
    QString name_;
    QString path_;
    PathsSelectionWizardPage* pathsPage_;
    FilesSelectionWizardPage* filesPage_;
};

class PathsSelectionWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PathsSelectionWizardPage(OpenProjectWizard* wizard);

    QString projectName() const;
    void setProjectName(QString const& name);

private:
    OpenProjectWizard* wizard_;
    QLineEdit* nameLineEdit_;
};

#if 0
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
