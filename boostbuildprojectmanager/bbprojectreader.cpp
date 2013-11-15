//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbprojectreader.hpp"
#include "bbprojectmanagerconstants.hpp"
#include "bbutility.hpp"
// Qt Creator
#include <coreplugin/progressmanager/progressmanager.h>
#include <utils/QtConcurrentTools>
// Qt
#include <QDir>
#include <QString>
#include <QStringList>

namespace BoostBuildProjectManager {
namespace Internal {

ProjectReader::ProjectReader(QString const& projectPath)
    : projectPath_(projectPath)
    , futureCount_(0)
{
}

void ProjectReader::startReading()
{
    QFuture<void> task = QtConcurrent::run(&ProjectReader::run, this);
    Core::ProgressManager::addTask(task, tr("Reading"), Constants::TASK_PROJECT_READING);
}

void ProjectReader::buildFileListFinished()
{
    BBPM_QDEBUG("found " << files_.size() << " files");
    emit readingFinished();
}

void ProjectReader::run(QFutureInterface<void>& future)
{
    futureCount_ = 0;
    files_.clear();
    buildFilesList(projectPath_, future);
}

void ProjectReader::buildFilesList(QString const& basePath
                                 , QFutureInterface<void>& future)
{
    QFileInfoList const fileInfoList =
        QDir(basePath).entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (QFileInfo const& fileInfo, fileInfoList)
    {
        if (futureCount_ % 100)
        {
            if (future.isCanceled())
                return;
        }

        if (fileInfo.isDir())
        {
            if (fileInfo.isSymLink())
                continue;

            QString const filePath = fileInfo.filePath();
            buildFilesList(filePath, future);

            BBPM_QDEBUG(filePath);
            files_.append(filePath);
        }
        else
        {
            QString const filePath(fileInfo.absoluteFilePath());
            BBPM_QDEBUG(filePath);
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
