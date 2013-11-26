Qt Creator Plugin for Boost.Build
==============================================================================

A [Boost.Build](http://www.boost.org/boost-build2/) project manager plugin for 
[Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).

Overview
------------------------------------------------------------------------------

The idea of the Boost.Build plugin is extremely simple - three main features:

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
------------------------------------------------------------------------------

Generally, follow the 
[Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html)
section from the [Creating Plugins](http://doc-snapshot.qt-project.org/qtcreator-extending/creating-plugins.html) guide.

Specifically, I used Qt and Qt Creator built from sources in their Git repositories:

* Qt 5 from branch: stable
* Qt Creator from branch: 3.0

So, I recommend to follow these guides:

1. [Building Qt 5 from Git](http://qt-project.org/wiki/Building_Qt_5_from_Git)
1. [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)

Authors
------------------------------------------------------------------------------

* [Mateusz Loskot](http://mateusz.loskot.net) (mateusz@loskot.net)

Credits
------------------------------------------------------------------------------

* Christian Kandeler, Tobias Hunger, Koehne Kai, André Pönitz for great help on the qt-creator mailing list,
* [Tomasz Siekierda](http://sierdzio.com/) for 
  [writing](http://lists.qt-project.org/pipermail/qt-creator/2013-October/002859.html)
  the [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)
  and support through qt-creator list,
* Vladimir Prus for answering questions about Boost.Build on #boost IRC channel,

lots of thanks to!

FAQ
------------------------------------------------------------------------------

Future
------------------------------------------------------------------------------

An unprioritised TODO lis for the plugin:

* Collect and resolve include paths as specified in all Jamfiles in project tree.
* Add support for parallel tree with header files, like Boost itself.
  Currently, in order to resolve includes from ```$BOOSTROOT``` for projects in ```$BOOSTROOT/libs```
  user needs to manually add ```$BOOSTROOT``` to ```*.qtcreator.includes``` file.
* Collect preprocessor defines from all project Jamfiles.
* Look for user-config.jam to determine some
* Boost.Build Jamfile parser to enable fully-featured project management capacity by
  * use b2 for [reporting include paths for a project](http://lists.boost.org/boost-build/2013/11/27173.php),
    targets, preprocessor defines and build configuration settings.
  * extracting parser from [tools/build/v2/engine](https://github.com/boostorg/build/tree/master/v2/engine)
    as reusable library
  * making some love with [Boost.Spirit](http://boost.org/libs/spirit)
