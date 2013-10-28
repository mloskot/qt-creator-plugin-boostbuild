#ifndef BBPROJECTFILE_HPP_INCLUDED
#define BBPROJECTFILE_HPP_INCLUDED

#include <coreplugin/idocument.h>

namespace BoostBuildProjectManager {
namespace Internal {

class Project;

// Describes the root of a project for use in Project class.
// In the context of Boost.Build the implementation is mostly empty,
// as the modification of a project is done by editing Jamfile.v2 files.
class ProjectFile : public Core::IDocument
{
    Q_OBJECT

public:
    ProjectFile(Project* project, QString const& fileName);

    bool save(QString* errorString, QString const& fileName, bool autoSave);
    QString defaultPath() const;
    QString suggestedFileName() const;
    QString mimeType() const;
    bool isModified() const;
    bool isSaveAsAllowed() const;
    bool reload(QString* errorString, ReloadFlag flag, ChangeType type);

private:
    Project* project_;
};

} // namespace Internal
} // namespace AutotoolsProjectManager

#endif // BBPROJECTFILE_HPP_INCLUDED
