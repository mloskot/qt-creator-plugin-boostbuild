Qt Creator Plugin for Boost.Build
=================================

The idea is extremely simple:

* **Open** Jamfile as a project in Qt Creator

* Run **Build**

* **Parse** ```b2``` command output to enable jumps to file/line of compilation error 
  * It turns out, that this simple feature has grown to more beefy requirements of 
    project loading, project tree reading, build configuration and build steps juggling.

The plugin is **not** supposed to provide:

* fully-featured parser for Jamfiles
  
* any kind of GUI for editing, updating or managing configuration stored in Jamfiles

Building
--------

Generally, follow the 
[Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html)
section from the [Creating Plugins](http://doc-snapshot.qt-project.org/qtcreator-extending/creating-plugins.html) guide.

Specifically, I used Qt and Qt Creator built from sources in their Git repositories:

* Qt 5 in branch: stable
* Qt Creator branch: 3.0

So, I recommend to follow these guides:

1. [Building Qt 5 from Git](http://qt-project.org/wiki/Building_Qt_5_from_Git)
1. [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)

Credits
-------

* André Pönitz for help on qt-creator mailing list
* Koehne Kai for licensing advise and support on the mailing list.
* Tobias Hunger for answering Qt Creator API questions on the mailing list.
* [Tomasz Siekierda](http://sierdzio.com/) for [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)
  and support through qt-creator list.

Author
------

* Mateusz Loskot (mateusz@loskot.net)