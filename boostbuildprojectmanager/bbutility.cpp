//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
#include "bbutility.hpp"
// Qt Creator
#include <utils/fileutils.h>
#include <utils/qtcassert.h>
// Qt
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QList>
#include <QRegExp>
#include <QSet>
#include <QString>
#include <QStringList>

namespace BoostBuildProjectManager {
namespace Utility {

QStringList readLines(QString const& filePath)
{
    QFileInfo const fileInfo(filePath);
    QStringList lines;

    QFile file(fileInfo.absoluteFilePath());
    if (file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);

        forever
        {
            QString line = stream.readLine();
            if (line.isNull())
                break;

            lines.append(line);
        }
    }

    return lines;
}

QStringList makeAbsolutePaths(QString const& basePath, QStringList const& paths)
{
    // TODO: QDir baseDir(QFileInfo(basePath)....
    QDir const baseDir(basePath);

    QFileInfo fileInfo;
    QStringList absolutePaths;
    foreach (QString const& path, paths)
    {
        QString trimmedPath = path.trimmed();
        if (!trimmedPath.isEmpty())
        {
            trimmedPath = Utils::FileName::fromUserInput(trimmedPath).toString();

            fileInfo.setFile(baseDir, trimmedPath);
            if (fileInfo.exists())
            {
                QString const absPath = fileInfo.absoluteFilePath();
                Q_ASSERT(!absPath.isEmpty());
                absolutePaths.append(absPath);
            }
        }
    }
    absolutePaths.removeDuplicates();
    return absolutePaths;
}

QStringList& makeRelativePaths(QString const& basePath, QStringList& paths)
{
    QDir const baseDir(basePath);
    for (QStringList::iterator it = paths.begin(), end = paths.end(); it != end; ++it)
        *it = baseDir.relativeFilePath(*it);
    return paths;
}

QHash<QString, QStringList> sortFilesIntoPaths(QString const& basePath
                                             , QSet<QString> const& files)
{
    QHash<QString, QStringList> filesInPath;
    QDir const baseDir(basePath);

    foreach (QString const& absoluteFileName, files)
    {
        QFileInfo const fileInfo(absoluteFileName);
        Utils::FileName absoluteFilePath = Utils::FileName::fromString(fileInfo.path());
        QString relativeFilePath;

        if (absoluteFilePath.isChildOf(baseDir))
        {
            relativeFilePath = absoluteFilePath.relativeChildPath(
                        Utils::FileName::fromString(basePath)).toString();
        }
        else
        {
            // `file' is not part of the project.
            relativeFilePath = baseDir.relativeFilePath(absoluteFilePath.toString());
            if (relativeFilePath.endsWith(QLatin1Char('/')))
                relativeFilePath.chop(1);
        }

        filesInPath[relativeFilePath].append(absoluteFileName);
    }
    return filesInPath;
}

QString parseJamfileProjectName(QString const& fileName)
{
    QString projectName;
    QFile file(fileName);
    if (file.exists())
    {
        // Jamfile project rule tokens to search for
        QString const projectBegin(QLatin1String("project"));
        QChar const projectSep(QLatin1Char(':'));
        QChar const projectEnd(QLatin1Char(';'));
        QChar const projectS(QLatin1Char(';'));
        QString projectDef; // buffer for complete project definition

        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString const line(stream.readLine());
            if (projectDef.isEmpty() && line.trimmed().startsWith(projectBegin))
                projectDef.append(line);

            if (!projectDef.isEmpty())
            {
                if (projectDef != line)
                {
                    if (line.isEmpty())
                        projectDef.append(QLatin1Char(' '));
                    else
                        projectDef.append(line);
                }
            }

            if (line.contains(projectSep) || line.contains(projectEnd))
                break;
        }

        if (!projectDef.isEmpty())
        {
            QRegExp rx(QLatin1String("\\s*project\\s+([a-zA-Z\\-\\/]+)\\s+[\\:\\;]"));
            rx.setMinimal(true);
            QTC_CHECK(rx.isValid());
            if (rx.indexIn(projectDef) > -1)
            {
                projectName = rx.cap(1);
            }
        }
    }
    return projectName;
}

} // namespace Utility
} // namespace BoostBuildProjectManager
