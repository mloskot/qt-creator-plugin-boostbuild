//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbprojectreader.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <coreplugin/mimedatabase.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <utils/QtConcurrentTools>
// Qt
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QStringList>

namespace BoostBuildProjectManager {
namespace Internal {

ProjectReader::ProjectReader(QString const& projectPath)
    : projectPath_(projectPath)
    , futureCount_(0)
{
    Q_ASSERT(QFileInfo(projectPath).isDir());
    BBPM_QDEBUG(projectPath_);
}

void ProjectReader::startReading()
{
    BBPM_QDEBUG(projectPath_);

    connect(&futureWatcher_, SIGNAL(finished()), this, SLOT(handleReadingFinished()));

    QFuture<void> task = QtConcurrent::run(&ProjectReader::run, this);
    futureWatcher_.setFuture(task);
    Core::ProgressManager::addTask(task, tr("Reading"), Constants::TASK_PROJECT_READING);
}

void ProjectReader::handleReadingFinished()
{
    if (futureWatcher_.isCanceled())
    {
        BBPM_QDEBUG("canceled");
        return;
    }

    BBPM_QDEBUG("found " << files_.size() << " files");
    emit readingFinished();

}

void ProjectReader::run(QFutureInterface<void>& future)
{
    future.setProgressRange(0, 0);
    future.reportStarted();

    futureCount_ = 0;
    files_.clear();

    // TODO: refine suffixes: all text only or specific C/C++ types
    QStringList const suffixes = Core::MimeDatabase::suffixes();
    buildFilesList(projectPath_, suffixes, future);

    future.reportFinished();
}

void ProjectReader::buildFilesList(QString const& basePath
                                 , QStringList const& suffixes
                                 , QFutureInterface<void>& future)
{
    Q_ASSERT(!suffixes.isEmpty());

    QFileInfoList const fileInfoList =
        QDir(basePath).entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (QFileInfo const& fileInfo, fileInfoList)
    {
        if (futureCount_ % 100)
        {
            if (future.isCanceled())
                return;
            future.setProgressValue(files_.size());
        }
        ++futureCount_;

        if (fileInfo.isDir())
        {
            if (fileInfo.isSymLink())
                continue;

            QString const filePath= fileInfo.filePath();
            buildFilesList(filePath, suffixes, future);

            // TODO: Performance of this test?!
            if (!files_.contains(filePath))
                files_.append(filePath);
        }
        else
        {
            QString const filePath = fileInfo.absoluteFilePath();

            // Add only files of requested types
            if (suffixes.contains(fileInfo.suffix()))
                files_.append(filePath);
        }


    }
}

QStringList ProjectReader::files() const
{
    return files_;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
