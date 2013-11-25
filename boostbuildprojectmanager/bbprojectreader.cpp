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
    includePaths_.clear();

    // TODO: refine suffixes: all text only or specific C/C++ types
    QStringList const suffixes = Core::MimeDatabase::suffixes();

    QStringList headerFilters;
    QStringList const headerMimes
        = QStringList() << QLatin1String("text/x-chdr") << QLatin1String("text/x-c++hdr");
    foreach (QString const& headerMime, headerMimes)
    {
        Core::MimeType mime = Core::MimeDatabase::findByType(headerMime);
        foreach (Core::MimeGlobPattern const& gp, mime.globPatterns())
            headerFilters.append(gp.pattern());
    }

    buildFilesList(projectPath_, suffixes, headerFilters, future);

    future.reportFinished();
}

void ProjectReader::buildFilesList(QString const& basePath
                                 , QStringList const& suffixes
                                 , QStringList const& headerFilters
                                 , QFutureInterface<void>& future)
{
    Q_ASSERT(!suffixes.isEmpty());
    Q_ASSERT(!headerFilters.isEmpty());

    QDir const projectDir(projectPath_);
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

            // TODO: pass names to filter as arg
            // "Since Boost.Build almost always generates tergets under the "bin"
            // directiry" (see Boost.Build architecture doc),
            // this as well as "bin.v2" and other variations
            // should be skipped as not a part of project tree
            if (fileInfo.baseName() == QLatin1String("bin"))
                continue;

            // If any C/C++ headers in this directory, add it to include paths,
            // used for C/C++ parsing only.
            QDir const thisDir(fileInfo.dir());
            if (!thisDir.entryList(headerFilters, QDir::Files).isEmpty())
            {
                QString const relative = projectDir.relativeFilePath(thisDir.path());
                includePaths_.append(relative.isEmpty() ? QLatin1String(".") : relative);
            }

            // Continue looking for sources
            buildFilesList(fileInfo.filePath(), suffixes, headerFilters, future);
        }
        else
        {
            // Add file to sources list, only of requested types.
            QString const filePath = fileInfo.absoluteFilePath();
            if (suffixes.contains(fileInfo.suffix()))
            {
                // TODO: Performance of this test?!
                if (!files_.contains(filePath))
                    files_.append(filePath);
            }
        }


    }
}

QStringList ProjectReader::files() const
{
    return files_;
}

QStringList ProjectReader::includePaths() const
{
    return includePaths_;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
