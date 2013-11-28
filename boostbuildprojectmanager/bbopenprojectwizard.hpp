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

class FilesSelectionWizardPage;

class OpenProjectWizard : public Utils::Wizard
{
    Q_OBJECT

public:
    OpenProjectWizard(QWidget* parent = 0);

    QString path() const;
    void setPath(QString const& path);
    QStringList selectedFiles() const;
    QStringList selectedPaths() const;

    QString projectName() const;

    PathsSelectionWizardPage* pathsPage_;
    FilesSelectionWizardPage* filesPage_;
};

class PathsSelectionWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PathsSelectionWizardPage(OpenProjectWizard* wizard);

private:
    OpenProjectWizard* wizard_;
    QLineEdit* name_;
    Utils::PathChooser* workPathChooser_;
    Utils::PathChooser* buildPathChooser_;
};

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

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBOPENPROJECTWIZARD_HPP
