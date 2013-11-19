#include "bboutputparser.hpp"
#include "bbutility.hpp"
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

BoostBuildParser::BoostBuildParser()
{
}


void BoostBuildParser::setToolsetParser(QString const& line)
{
    parser_ = false;
    if (!parser_)
    {
        QRegExp rxToolset(QLatin1String("^([a-z\\-]+)\\."));
        rxToolset.setMinimal(true);
        QTC_CHECK(rxToolset.isValid());

        if (rxToolset.indexIn(line) > -1)
        {
            BBPM_QDEBUG(rxToolset.cap(0));
            QString const toolsetName(rxToolset.cap(0));

            if (QStringRef(&toolsetName, 0, 3) == QLatin1String("gcc"))
            {
            }
            else if (QStringRef(&toolsetName, 0, 5) == QLatin1String("clang"))
            {
            }
            else
            {
                BBPM_QDEBUG("toolset unknown");
            }
        }
    }
}

void BoostBuildParser::stdOutput(QString const& line)
{
    setToolsetParser(line);

    QString const rline = rightTrimmed(line);

    ProjectExplorer::IOutputParser::stdOutput(line);
}

void BoostBuildParser::stdError(QString const& line)
{
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
