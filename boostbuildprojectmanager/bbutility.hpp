//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
//
// This file is part of Qt Creator Boost.Build plugin project.
//
// This is free software; you can redistribute and/or modify it under
// the terms of the GNU Lesser General Public License, Version 2.1
// as published by the Free Software Foundation.
// See the LICENSE.txt file for more information.
//
#ifndef BBUTILITY_HPP
#define BBUTILITY_HPP
#ifdef _DEBUG

#include "bbprojectmanagerconstants.hpp"
// Qt
#include <QDebug>
#include <QHash>
#include <QSet>
#include <QString>
#include <QStringList>

//////////////////////////////////////////////////////////////////////////////////////////

#define BBPM_QDEBUG(msg) \
    qDebug() \
        << "[" << BoostBuildProjectManager::Constants::BOOSTBUILD << "] " \
        << "(" << __PRETTY_FUNCTION__ << ")"; \
    qDebug().nospace() << "\t" << msg

#else
nospace
#define BBPM_QDEBUG(msg)

#endif // _DEBUG

#define BBPM_C(CONSTANT) QLatin1String(BoostBuildProjectManager::Constants::CONSTANT)

//////////////////////////////////////////////////////////////////////////////////////////
namespace BoostBuildProjectManager {
namespace Utility {

// Read all lines from a file.
QStringList readLines(QString const& absoluteFileName);

// Converts the path from relative to the project to an absolute path.
QStringList makeAbsolutePaths(QString const& basePath, QStringList const& paths);

QStringList& makeRelativePaths(QString const& basePath, QStringList& paths);

QHash<QString, QStringList> sortFilesIntoPaths(QString const& basePath
                                             , QSet<QString> const& files);

QString parseJamfileProjectName(QString const& fileName);

} // namespace Utility
} // namespace BoostBuildProjectManager

#endif // BBUTILITY_HPP
