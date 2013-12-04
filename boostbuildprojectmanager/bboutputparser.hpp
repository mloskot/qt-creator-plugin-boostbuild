//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
//
// This file is part of Qt Creator Boost.Build plugin project.
//
// This is free software; you can redistribute and/or modify it under
// the terms of the GNU Lesser General Public Licence as published
// by the Free Software Foundation.
// See the LICENSE.txt file for more information.
//
#ifndef BBOUTPUTPARSER_HPP
#define BBOUTPUTPARSER_HPP

// Qt Creator
#include <projectexplorer/ioutputparser.h>
#include <projectexplorer/task.h>
// Qt
#include <QPointer>
#include <QString>

namespace BoostBuildProjectManager {
namespace Internal {

class BoostBuildParser : public ProjectExplorer::IOutputParser
{
    Q_OBJECT

public:
    BoostBuildParser();

    void stdOutput(QString const& line);
    void stdError(QString const& line);

protected:
    void doFlush();

private:

    QString findToolset(QString const& line) const;
    void setToolsetParser(QString const& toolsetName);

    QString toolsetName_;
    QRegExp toolsetNameReCommand_; // matches line with ".compile." command
    QRegExp toolsetNameReWarning_; // matches line with "warning: " status

    ProjectExplorer::Task lastTask_;
    QPointer<ProjectExplorer::IOutputParser> parser_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBOUTPUTPARSER_HPP
