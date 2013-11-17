# Development notes

There are several auxiliary files generated or used by the Boost.Build project manager to accompany *Jamfile*:

* *.files* - caches list of files (generated, always)
* *.includes* - list of include paths (provided by user, optional)

The auxiliary files should to use extensions indicating they work with Qt Creator only:

* *.files* file uses *.qtcreator.files*
* *.includes* file uses *.qtcreator.includes*

## Open project flow

Open Project dialog triggers sequence of calls: 
`ProjectExplorerPlugin::openProject` > `Project::fromMap` > `ProjectExplorer::Project::fromMap`
which should perform:

* reading or refreshing?
* *.files* file
    1. Does .files file exist? * No, go to 2. Yes, go to 3.
    2. `ProjectReader` creates *.files* file. Go to 1.
    3. Read *.files* file lines. Go to 4.
    4. Generate project node tree.
    5. *Jamfile* changed? Yes, go to 2.
       Note, changes in non-file configuration or files managed. No way to know, so refreshing the tree
* *.includes* file
    1. Does *.includes* file exist? Yes, go to 2. No, go to ?.
    2. Read *.includes* file lines.
* Configure `CppTools` and schedule C++ file sfor parsing.
    * Use paths read from *.includes* file.
* `setActiveTarget` and `setActiveBuildConfiguration`
