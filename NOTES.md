 # Development notes

## Open project flow

* ProjectExplorerPlugin::openProject
  * Project::fromMap
    * ProjectExplorer::Project::fromMap
      * setActiveTarget and setActiveBuildConfiguration
      * reading or refreshing?
      * .files file
        1. Does .files file exist? * No, go to 2. Yes, go to 3.
        2. ProjectReader creates .files file. Go to 1.
        3. Read .files file lines. Go to 4.
        4. Generate project node tree.
        6. Jamfile changed? Yes, go to 2. Note, changes in non-file configuration or
           files managed. No way to know, so refreshing the tree
      * .includes file
        1. Does .includes file exist? Yes, go to 2. No, go to ?.
        2. Read .includes file lines.
      * Configure CppTools and schedule C++ file sfor parsing.
        * Use paths read from .includes file.
