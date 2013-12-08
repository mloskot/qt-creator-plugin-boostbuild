//
// Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
//
// This file is part of Qt Creator Boost.Build plugin project.
//
// This is free software; you can redistribute and/or modify it under
// the terms of the  GNU Lesser General Public License, Version 2.1
// as published by the Free Software Foundation.
// See accompanying file LICENSE.txt or copy at 
// http://www.gnu.org/licenses/lgpl-2.1-standalone.html.
//
#include "bboutputparser.hpp"
#include "bbutility.hpp"
#include "external/projectexplorer/gccparser.h"
#include "external/projectexplorer/ldparser.h"
#include "external/projectexplorer/clangparser.h"
// Qt Creator
#include <projectexplorer/ioutputparser.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/task.h>
#include <utils/qtcassert.h>
// Qt
#include <QRegExp>
#include <QString>
#include <QStringRef>

namespace BoostBuildProjectManager {
namespace Internal {

namespace {
char const* const RxToolsetFromCommand = "^([\\w-]+)(?:\\.)([\\w-]+).+$";
char const* const RxToolsetFromWarning = "^warning\\:.+toolset.+\\\"([\\w-]+)\\\".*$";
char const* const RxTestPassed = "^\\*\\*(\\w+)\\*\\*.+\\W(\\w+\\.test)";
}

BoostBuildParser::BoostBuildParser()
    : rxToolsetNameCommand_(QLatin1String(RxToolsetFromCommand))
    , rxToolsetNameWarning_(QLatin1String(RxToolsetFromWarning))
    , rxTestPassed_(QLatin1String(RxTestPassed))
    , lineMode_(Common)
{
    rxToolsetNameCommand_.setMinimal(true);
    QTC_CHECK(rxToolsetNameCommand_.isValid());

    rxToolsetNameWarning_.setMinimal(true);
    QTC_CHECK(rxToolsetNameWarning_.isValid());

    rxTestPassed_.setMinimal(true);
    QTC_CHECK(rxTestPassed_.isValid());
}

QString BoostBuildParser::findToolset(QString const& line) const
{
    QString name;
    if (rxToolsetNameWarning_.indexIn(line) > -1)
        name = rxToolsetNameWarning_.cap(1);
    else if (rxToolsetNameCommand_.indexIn(line) > -1)
        name = rxToolsetNameCommand_.cap(1);
    return name;
}

void BoostBuildParser::setToolsetParser(QString const& toolsetName)
{
    if (!toolsetName.isEmpty() && toolsetName_.isEmpty())
    {
        if (QStringRef(&toolsetName, 0, 3) == QLatin1String("gcc"))
        {
            appendOutputParser(new ProjectExplorer::GccParser());
            toolsetName_ = toolsetName;
        }
        else if (QStringRef(&toolsetName, 0, 5) == QLatin1String("clang"))
        {
            // clang-xxx found (e.g. clang-linux)
            appendOutputParser(new ProjectExplorer::ClangParser());
            toolsetName_ = toolsetName;
        }
        else
        {
            BBPM_QDEBUG("TODO: Add more toolsets (Intel, VC++, etc.)");
        }
    }
}

void BoostBuildParser::stdOutput(QString const& rawLine)
{
    QString const line = rightTrimmed(rawLine);
    setToolsetParser(findToolset(line));

    if (!toolsetName_.isEmpty() && line.startsWith(toolsetName_))
        lineMode_ = Toolset;
    else if (line.startsWith(QLatin1String("testing")))
        lineMode_ = Testing;
    else if (line.startsWith(QLatin1String("common")))
        lineMode_ = Common;

    // TODO: Handle (failed-as-expected) - cancel error Task

    if (lineMode_ == Toolset)
    {
        // Boost.Build seems to send everything to stdout,
        // whereas gcc and clang to use stderr.
        ProjectExplorer::IOutputParser::stdError(line);
    }
    else if (lineMode_ == Testing)
    {

        if (rxTestPassed_.indexIn(line) > -1)
        {
            BBPM_QDEBUG(rxTestPassed_.capturedTexts());

            QString const testStatus = rxTestPassed_.cap(1);
            QString const testName = rxTestPassed_.cap(2);
            if (testStatus == QLatin1String("passed"))
            {
                ProjectExplorer::Task task(ProjectExplorer::Task::Unknown
                    , testName
                    , Utils::FileName()
                    , -1 // line
                    , ProjectExplorer::Constants::TASK_CATEGORY_COMPILE);

                QTextLayout::FormatRange fr;
                fr.format.setForeground(QBrush(Qt::green));
                task.formats.append(fr);

                setTask(task);
            }

            lineMode_ = Common;
        }
    }
    else
    {

        doFlush();
        ProjectExplorer::IOutputParser::stdOutput(line);
    }
}

void BoostBuildParser::stdError(QString const& line)
{
    setToolsetParser(findToolset(line));
    ProjectExplorer::IOutputParser::stdError(line);
}

void BoostBuildParser::doFlush()
{
    if (!lastTask_.isNull())
    {
        ProjectExplorer::Task t = lastTask_;
        lastTask_.clear();
        emit addTask(t);
    }
}

void BoostBuildParser::setTask(ProjectExplorer::Task const& task)
{
    doFlush();
    lastTask_ = task;
}

} // namespace Internal
} // namespace BoostBuildProjectManager
