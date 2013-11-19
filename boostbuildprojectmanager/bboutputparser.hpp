#ifndef BBOUTPUTPARSER_H_INCLUDED
#define BBOUTPUTPARSER_H_INCLUDED

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

    void setToolsetParser(QString const& line);

    ProjectExplorer::Task lastTask_;
    //QPointer<ProjectExplorer::IOutputParser> parser_;
    bool parser_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBOUTPUTPARSER_H_INCLUDED
