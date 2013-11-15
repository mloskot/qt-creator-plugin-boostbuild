#ifndef BBPROJECTTREELOADER_HPP_INCLUDED
#define BBPROJECTTREELOADER_HPP_INCLUDED

// Qt
#include <QFutureInterface>
#include <QFutureWatcher>
#include <QString>
#include <QStringList>

namespace BoostBuildProjectManager {
namespace Internal {

class ProjectReader : public QObject
{
    Q_OBJECT
public:
    ProjectReader(QString const& projectPath);

    void startReading();
    QStringList files() const;


signals:
    void readingFinished();
    void readingProgress(QString const& fileName);

private slots:
    void buildFileListFinished();

private:
    void run(QFutureInterface<void>& future);
    void buildFilesList(QString const& baseDir, QFutureInterface<void>& future);

    QString projectPath_;
    QStringList files_;
    QFutureWatcher<void> futureWatcher_;
    int futureCount_;
};

} // namespace Internal
} // namespace BoostBuildProjectManager

#endif // BBPROJECTTREELOADER_HPP_INCLUDED
