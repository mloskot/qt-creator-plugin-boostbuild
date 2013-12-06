Boost.Build Plugin for Qt Creator
==============================================================================

A plugin with basic [Boost.Build](http://www.boost.org/boost-build2/) support
for [Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).

https://github.com/mloskot/qt-creator-plugin-boostbuild

Overview
------------------------------------------------------------------------------

The idea of the Boost.Build plugin is extremely simple - use Qt Creator as
a basic GUI shell to interact with the Boost.Build command.

Three main features:

* **Open** ```Jamroot``` or ```Jamfile.v2``` as a project in Qt Creator.
* **Build** project in Qt Creator as it is configured by its Jamfile(s).
* **Parse** Boost.Build command output to enable navigation to error or warning locations.

Think of the plugin as an equivalent of Vim or Emacs with ctags and bunch of other scripts.

It turns out, that in order to achieve the three basic features above,
a project manager plugin for Qt Creator needs to implement a few beefy features like:
opening a project, reading project directory structure, creating nodes of project 
explorer tree, creating build, run, deploy configurations and
corresponding build, run, deploy steps and more.

So, the Boost.Build plugin implements as little as necessary,
and currently, it does **not** provide:

* any parsing capabilities for Boost.Build Jamfiles (not entirely true, see [Features](#Features))
* any kind of GUI for updating or managing configuration stored in Jamfiles

Authors
------------------------------------------------------------------------------

* [Mateusz Łoskot](http://mateusz.loskot.net) (mateusz@loskot.net)

Credits
------------------------------------------------------------------------------

* Tobias Hunger, Koehne Kai, Christian Kandeler, Nikolai Kosjar, André Pönitz and others
  for great help on the qt-creator mailing list and IRC,
* [Tomasz Siekierda](http://sierdzio.com/) for 
  [writing](http://lists.qt-project.org/pipermail/qt-creator/2013-October/002859.html) the
  [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)
  and support through qt-creator list,
* Vladimir Prus for answering questions about Boost.Build,
* Authors of existing project manager plugins for Qt Creator - prime 
  learning resource,

lots of thanks to!

License
------------------------------------------------------------------------------

* [GNU Lesser General Public License, Version 2.1](http://www.gnu.org/licenses/lgpl-2.1.html)

- - -

Features
------------------------------------------------------------------------------

What the Boost.Build plugin can do?

* Select a Jamfile at any level of a source tree configured with Boost.Build.
* Select files at any level of a source tree and register them as project files.
* Open Jamfile pretending it is a Qt Creator project file.
* Parse selected Jamfile to retrieve project name, if defined. Otherwise, Jamfile parent
  directory name is used as a project name. Finally, project name can be freely changed.
* Generate list of include paths based on configured project files tree.
* Utilise C++ browsing and editing capabilities and features of Qt Creator.
* Run Boost.Build command ```b2``` from within Qt Creator.
* Configure Boost.Build command arguments.
* Configure Boost.Build command working directory where ```b2``` is executed.
* Configure Boost.Build command build directory with ```b2``` option ```--build-dir```.
* Parse Boost.Build command and Boost.Build toolset output to generate Qt Creator's
  Issues list and allow navigation to file and line of error or warning.
    * Currently, GCC and Clang parsers are provided.
* Configure build and clean steps.
* Configure run targets for testing and debugging.
* Save all Qt Creator project configuration changes in sidecar file ```.user```.
* Modify and save registered project files in sidecar file ```.qtcreator.files```.
* Modify and save includes paths in sidecar file ```.qtcreator.includes```..
* ...

What the Boost.Build plugin can not do?

* Parse Jamfiles to collect list of source files which are actually used.
* Parse Jamfiles to collect configuration of targets, attributes, requirements, etc.
* Capture any project changes made in Qt Creator UI and automatically update Jamfiles.
* Change any Boost.Build configuration.
* ...

Installation
------------------------------------------------------------------------------

You need to build the plugin from sources.

Generally, follow the 
[Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html)
section from the
[Creating Plugins](http://doc-snapshot.qt-project.org/qtcreator-extending/creating-plugins.html)
guide.

I use Qt and Qt Creator built from sources in their Git repositories:

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

See also the [Installation](#installation) section.

### How to install the plugin?

[Build](#building) it.

### Do I need to build Qt?

I don't know. Ask Qt folks.

I follow their [Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html):

> Qt Creator requires private headers of Qt, which are unfortunately
> not installed by the Qt binary packages

### Do I need to build Qt Creator?

Apparently, yes, as per [Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html):

> It is also necessary if you want to create your own Qt Creator plugin.

### What is the roadmap?

None, apart form plan to roll the project forward fixing bugs and adding features
as they appear useful or necessary in order to make work with Boost.Build-based
a bit more accessible and efficient.

See [Future](#future) section for some feature ideas.

### How can I contribute?

[Fork the repo](https://help.github.com/articles/fork-a-repo) and make the plugin better!


### Have you used it on Windows?

No.

### Have you used it on Mac OS X?

No.

Future
------------------------------------------------------------------------------

An unprioritised TODO list for the plugin:

* Add tests! Add tests! Add tests! But first, learn how to test with Qt wealth!
* Parse and colour test output (failed in red, passed in green)
* Add file watchers for Jamfile and sidecar files to automatically reload any changes.
* Allow to specify ```b2``` command location not in ```PATH```.
* Add ```DeployConfiguration``` and ```RunConfiguration``` targets, if really needed.
* Determine toolset name from Kit and add ```toolset``` to additional arguments of step.
    * See TODO item about ```user-config.jam``` lookup.
* Improve use of [Qt Creator’s C++ model](http://ltcmelo.com/blog/2013/03/17/a-bit-about-qt-creators-c-model/)
  for better code browsing experience
    * Collect and resolve include paths as specified in all Jamfiles in project tree.
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

Search through the sources for ```TODO```, there is more items.
