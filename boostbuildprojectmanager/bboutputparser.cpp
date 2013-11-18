#include "bboutputparser.hpp"
// Qt Creator
#include <projectexplorer/ioutputparser.h>
#include <projectexplorer/task.h>
// Qt
#include <QString>

namespace BoostBuildProjectManager {
namespace Internal {

OutputParser::OutputParser()
{
}

void OutputParser::stdOutput(QString const& line)
{
    // TODO
    ProjectExplorer::IOutputParser::stdOutput(line);
}

void OutputParser::stdError(QString const& line)
{
    // TODO
    ProjectExplorer::IOutputParser::stdError(line);
}

void OutputParser::doFlush()
{
    if (!lastTask_.isNull())
    {
        ProjectExplorer::Task t = lastTask_;
        lastTask_.clear();
        emit addTask(t);
    }
}

} // namespace Internal
} // namespace BoostBuildProjectManager
