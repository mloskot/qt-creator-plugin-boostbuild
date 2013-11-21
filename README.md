Qt Creator Plugin for Boost.Build
=================================

A [Boost.Build](http://www.boost.org/boost-build2/) project manager plugin for 
[Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).

Overview
--------

The idea is extremely simple:

* **Open** a Jamfile as a project in Qt Creator
* **Build** a project in Qt Creator as it is configured by its Jamfile(s).
* **Parse** the Boost.Build command output to enable navigation to error or warning locations.

It turns out, that in order to achieve the three features above, a project manager for
Qt Creator needs to fulfill more beefy requirements of opening a project,
reading project directory structure, creating nodes of project explorer tree,
creating build configurations, creating build steps for build configurations and so on.

Regardless the Qt Creator requirements, the plugin is **not** supposed to provide:

* fully-featured parser for Jamfiles 
* any kind of GUI for editing, updating or managing configuration stored in Jamfiles

Building
--------

Generally, follow the 
[Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html)
section from the [Creating Plugins](http://doc-snapshot.qt-project.org/qtcreator-extending/creating-plugins.html) guide.

Specifically, I used Qt and Qt Creator built from sources in their Git repositories:

* Qt 5 from branch: stable
* Qt Creator from branch: 3.0

So, I recommend to follow these guides:

1. [Building Qt 5 from Git](http://qt-project.org/wiki/Building_Qt_5_from_Git)
1. [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)

Future
------

### TODO ###

An unprioritised list of TODO list for the plugin:
... 

### Ideas ###

A random list of potentially interesting ideas for the plugin:
...

Authors
-------

* [Mateusz Loskot](http://mateusz.loskot.net) (mateusz@loskot.net)

Credits
-------

* Christian Kandeler, Tobias Hunger, Koehne Kai, André Pönitz for help on the qt-creator mailing list.
* [Tomasz Siekierda](http://sierdzio.com/) for 
  [writing](http://lists.qt-project.org/pipermail/qt-creator/2013-October/002859.html)
  the [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)
  and support through qt-creator list.
