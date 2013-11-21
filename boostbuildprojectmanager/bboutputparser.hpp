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

    QString findToolset(QString const& line) const;
    void setToolsetParser(QString const& toolsetName);

    QString toolsetName_;
    QRegExp toolsetNameReC_; // matches line with ".compile." command
    QRegExp toolsetNameReW_; // matches line with "warning: " status

    ProjectExplorer::Task lastTask_;
    QPointer<ProjectExplorer::IOutputParser> parser_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBOUTPUTPARSER_H_INCLUDED
