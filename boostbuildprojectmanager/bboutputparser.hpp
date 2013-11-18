#ifndef BBOUTPUTPARSER_H_INCLUDED
#define BBOUTPUTPARSER_H_INCLUDED

// Qt Creator
#include <projectexplorer/ioutputparser.h>
#include <projectexplorer/task.h>
// Qt
#include <QString>

namespace BoostBuildProjectManager {
namespace Internal {

class OutputParser : public ProjectExplorer::IOutputParser
{
    Q_OBJECT

public:
    OutputParser();

    void stdOutput(QString const& line);
    void stdError(QString const& line);

protected:
    void doFlush();

private:
    ProjectExplorer::Task lastTask_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBOUTPUTPARSER_H_INCLUDED
