Qt Creator Plugin for Boost.Build
==============================================================================

A [Boost.Build](http://www.boost.org/boost-build2/) project manager plugin for 
[Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).

Overview
------------------------------------------------------------------------------

The idea of the Boost.Build plugin is extremely simple - use Qt Creator as
a basic GUI shell to interact with the Boost.Build command.

Three main features:

* **Open** ```Jamroot``` or ```Jamfile.v2``` as a project in Qt Creator.
* **Build** project in Qt Creator as it is configured by its Jamfile(s).
* **Parse** Boost.Build command output to enable navigation to error or warning locations.

It turns out, that in order to achieve the three basic features above,
a project manager plugin for Qt Creator needs to implement a few beefy features like:
opening a project, reading project directory structure, creating nodes of project 
explorer tree, creating build, run, deploy configurations and
corresponding build, run, deploy steps and more.

So, the Boost.Build plugin implements as little as necessary,
and currently, it does **not** provide:

* any parsing capabilities for Boost.Build Jamfiles (not true, see [Features](#Features))
* any kind of GUI for updating or managing configuration stored in Jamfiles

Authors
------------------------------------------------------------------------------

* [Mateusz Loskot](http://mateusz.loskot.net) (mateusz@loskot.net)

Credits
------------------------------------------------------------------------------

* Christian Kandeler, Tobias Hunger, Koehne Kai, André Pönitz for great help
  on the qt-creator mailing list,
* [Tomasz Siekierda](http://sierdzio.com/) for 
  [writing](http://lists.qt-project.org/pipermail/qt-creator/2013-October/002859.html) the
  [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)
  and support through qt-creator list,
* Vladimir Prus for answering questions about Boost.Build on #boost IRC channel,

lots of thanks to!

License
------------------------------------------------------------------------------

* [GNU Lesser General Public License, Version 2.1](http://www.gnu.org/licenses/lgpl-2.1.html)

- - -

Features
------------------------------------------------------------------------------

What the Boost.Build plugin can do for me?

* It can parse selected Jamfile to retrieve project name, if defined.

*TODO*


Installation
------------------------------------------------------------------------------

You need to build the plugin from sources.

Generally, follow the 
[Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html)
section from the
[Creating Plugins](http://doc-snapshot.qt-project.org/qtcreator-extending/creating-plugins.html)
guide.

Specifically, I used Qt and Qt Creator built from sources in their Git repositories:

* Qt 5 from branch: stable
* Qt Creator from branch: 3.0

So, I recommend to follow these guides:

1. [Building Qt 5 from Git](http://qt-project.org/wiki/Building_Qt_5_from_Git)
1. [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)

FAQ
------------------------------------------------------------------------------

### Why Boost.Build plugin for Qt Creator?

I always wanted to learn Qt programming.

When my son Hugo was born, I went on two weeks paternity leave and, somewhat inspired by the 
[Seven by Seven](http://pragprog.com/categories/7in7) idea, I decided to do an experiment:
*can I learn a new programming skills or work on an open source project having only 
a few 15-30 minutes short breaks over a day at my disposal?* 

Qt Creator is a fantastic and powerful IDE which is surprisingly easy and pleasent to extend.

Yet, apparently, it is much quicker to hack fairly usable Boost.Build support for
Qt Creator than develop [Boost.CMake](https://svn.boost.org/trac/boost/wiki/CMake)
and wait until it is approved by the Boost project.

### Is it part of Qt Creator?

No.

### Will it become part of Qt Creator?

No demand, no man power, no plans.

### What version of Qt Creator is supported?

I use Qt Creator 3.0 or later.
It should work with earlier versions too.
Please, drop me a line if you've tested it with Qt Creator 2.8 or earlier.

See also the [Installation](#Installation) section.

### How to install the plugin?

[Build](#Building) it.

### What is the roadmap?

None, apart form plan to roll the project forward fixing bugs and adding features
as they appear useful or necessary in order to make work with Boost.Build-based
a bit more accessible and efficient.

See [Future[(#Future) section for some feature ideas.

### How can I contribute?

[Fork the repo](https://help.github.com/articles/fork-a-repo) and make the plugin better!


### Have you tested it on Windows?

No.

### Have you tested it on Mac OS X?

No.

Future
------------------------------------------------------------------------------

An unprioritised TODO lis for the plugin:

* Allow to specify ```b2``` command location not in ```PATH```.
* Add ```DeployConfiguration``` and ```RunConfiguration``` targets, if really needed.
* Determine toolset name from Kit and add ```toolset``` to additional arguments of step.
    * See TODO item about ```user-config.jam``` lookup.
* Collect and resolve include paths as specified in all Jamfiles in project tree.
* Add support for parallel tree with header files, like Boost itself.
  Currently, in order to resolve includes from ```$BOOSTROOT``` for projects
  in ```$BOOSTROOT/libs``` user needs to manually add ```$BOOSTROOT```
  to ```*.qtcreator.includes``` file.
* Collect preprocessor defines from all project Jamfiles.
* Look for ```user-config.jam``` to determine some useful settings, projects that may be
  referenced, include paths.
    * Default toolset and use it for Kit selection/validation? See separate TODO item.
* Boost.Build Jamfile parser to enable fully-featured project management capacity by
    * use b2 for
      [reporting include paths for a project](http://lists.boost.org/boost-build/2013/11/27173.php),
      targets, preprocessor defines and build configuration settings.
    * extracting parser from
      [tools/build/v2/engine](https://github.com/boostorg/build/tree/master/v2/engine)
      as reusable library
    * making some love with [Boost.Spirit](http://boost.org/libs/spirit)
