//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
//
// This file is part of Qt Creator Boost.Build plugin project.
//
// This is free software; you can redistribute and/or modify it under
// the terms of the GNU Lesser General Public License, Version 2.1
// as published by the Free Software Foundation.
// See accompanying file LICENSE.txt or copy at 
// http://www.gnu.org/licenses/lgpl-2.1-standalone.html.
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
    void setTask(ProjectExplorer::Task const& task);

    QRegExp rxToolsetNameCommand_; // matches line with ".compile." command
    QRegExp rxToolsetNameWarning_; // matches line with "warning: " status
    QRegExp rxTestPassed_; // matches line with "**passed**" status

    QString toolsetName_;

    // Boost.Build command mode relates to first command token in line.
    enum LineMode { Common, Toolset, Testing };
    LineMode lineMode_;

    ProjectExplorer::Task lastTask_;
    QPointer<ProjectExplorer::IOutputParser> parser_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBOUTPUTPARSER_HPP
