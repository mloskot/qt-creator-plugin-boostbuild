Qt Creator Plugin for Boost.Build
=================================

The idea is extremely simple:

* **Open** Jamfile as a project in Qt Creator
* Run **Build**
* **Parse** ```b2``` command output to enable jumps to file/line of compilation error 
  * It turns out, that this simple feature has grown to more beefy
    requirements of project loading, project tree reading,
	build configuration and build steps juggling.

The plugin is **not** supposed to provide:
* fully-featured parser for Jamfiles
* any kind of GUI for editing, updating or managing configuration stored in Jamfiles