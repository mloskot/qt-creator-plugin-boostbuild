#include "bboutputparser.hpp"
#include "bbutility.hpp"
#include "external/projectexplorer/gccparser.h"
#include "external/projectexplorer/ldparser.h"
#include "external/projectexplorer/clangparser.h"
// Qt Creator
#include <projectexplorer/ioutputparser.h>
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
}

BoostBuildParser::BoostBuildParser()
    : toolsetNameReC_(QLatin1String(RxToolsetFromCommand))
    , toolsetNameReW_(QLatin1String(RxToolsetFromWarning))
{
    toolsetNameReC_.setMinimal(true);
    toolsetNameReW_.setMinimal(true);
    QTC_CHECK(toolsetNameReC_.isValid());
    QTC_CHECK(toolsetNameReW_.isValid());
}

QString BoostBuildParser::findToolset(QString const& line) const
{
    QString name;
    if (toolsetNameReW_.indexIn(line) > -1)
        name = toolsetNameReW_.cap(1);
    else if (toolsetNameReC_.indexIn(line) > -1)
        name = toolsetNameReC_.cap(1);
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
            // TOOD
        }

        BBPM_QDEBUG("matched " << toolsetName_);
    }
}

void BoostBuildParser::stdOutput(QString const& line)
{
    setToolsetParser(findToolset(line));
    ProjectExplorer::IOutputParser::stdError(line);
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

} // namespace Internal
} // namespace BoostBuildProjectManager
