Boost.Build Plugin for Qt Creator
==============================================================================

The Boost.Build (long name: **Boost.Build Project Manager**) plugin provides basic
[Boost.Build](http://www.boost.org/boost-build2/) support
for [Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).

Project at https://github.com/mloskot/qt-creator-plugin-boostbuild.

Overview
------------------------------------------------------------------------------

The idea of the Boost.Build plugin is extremely simple - use Qt Creator as
a basic GUI shell to interact with the Boost.Build command.

Three main features:

* **Open** Boost.Build *Jamfile* as a project in Qt Creator.
* **Build** project in Qt Creator according to its configuration in *Jamfile*.
* **Parse** Boost.Build command output to enable navigation to error or warning locations.

Think of the plugin as an equivalent of Vim or Emacs with ctags and bunch of other scripts.

It turns out, that in order to achieve the three basic features above,
a project manager plugin for Qt Creator needs to implement a few beefy features like:
opening a project, reading project directory structure, creating nodes of project 
explorer tree, creating build, run, deploy configurations and
corresponding build, run, deploy steps and more.

So, the Boost.Build plugin implements as little as necessary,
and currently, it does **not** provide:

* any parsing capabilities for Boost.Build Jamfiles (not entirely true, see [Features](#features))
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
* [Vladimir Prus](http://vladimirprus.com/) for answering questions about Boost.Build,
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
    * Including the fantastic [searching with the Locator](http://qt-project.org/doc/qtcreator-2.8/creator-editor-locator.html)
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

* Create new projects based on Boost.Build (simply, create *Jamfile* manually).
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

I use Qt and Qt Creator built from sources in their Git repositories, for example:

* Qt 5 from branch: stable
* Qt Creator from branch: 3.3

So, I recommend to follow these guides:

1. [Building Qt 5 from Git](http://qt-project.org/wiki/Building_Qt_5_from_Git)
1. [Building Qt Creator from Git](http://qt-project.org/wiki/Building-Qt-Creator-from-Git)

Once you have Qt Creator build ready, you need to set two environment variables:

* ```QTC_SOURCE``` - Qt Creator sources location (e.g. path to Git clone)
* ```QTC_BUILD``` - Qt Creator build or install prefix where the plugin will be deployed

You can either set the variables in CLI console, if you're going to run ```qmake```
directly or in Qt Creator, in
[Build Environment](http://doc-snapshot.qt-project.org/qtcreator-3.3/creator-build-settings.html)
settings of the ```boostbuildprojectmanager.pro``` project, after you open it in the IDE
in order to build the plugin from Qt Creator.

Quick Start
------------------------------------------------------------------------------

1. Go to **Open Project** and navigate to *Jamfile* of your project.
1. **Open Boost.Build Project** wizard opens
1. First page **Project Name and Paths** allows to inspect several paths and
   customise **Project name**.
1. Second page **Files Selection** allows to generate list of files to attach
   to the project node in Qt Creator **Projects** tree and to use as input for C++ model
   to enable C++ navigation features for the project.
1. Third page **Project Management** allows to add the project as a subproject, enable
   version control system and displays summary of auxiliary files generated by the wizard.
1. **Finish** the wizard and let Qt Creator to generate the project tree and parse all
   the source files attached generating C++ model of the code.
1. Go to **Projects** mode to inspect and customise variety of settings:
    * ```b2``` command working directory, that is where the command is executed
    * build directory given to ```b2``` as value of ```--build-dir``` option
    * build steps and arguments for ```b2``` command (e.g. change toolset, build variant)
    * add custom build steps
    * create run configuration with executable targets for testing and debugging
1. Also, you can edit ```.qtcreator.files``` to add or remove source files and edit
   ```.qtcreator.includes``` to specify include paths if C++ model is missing any.

For more details about individual features, steps and settings check the [FAQ](#faq).

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

### How does it compare to Generic Project plugin?

The Boost.Build plugin is inspired and largely based on the 
[Generic Project](http://doc-snapshot.qt-project.org/qtcreator-3.3/creator-project-generic.html) 
plugin (aka GenericProjectManager), both idea and implementation.

The Boost.Build plugin is a hybrid between "Open Project" and "Import Project" plugin.

Similarly to Generic Project support, the Boost.Build plugin:
* allows to use Qt Creator as a code editor,
* generates ```.files``` and ```.includes``` kinds of auxiliary files
* allows to control some of aspects of the way project is built by modifying the build
  command in the Projects mode under Build Settings
* needs to be told which executable to run in in the Projects mode under Run Settings,
  because Qt Creator cannot automatically determine it (see **TODO** items)

Unlike Generic Project support, the Boost.Build plugin:
* can only use Boost.Build system and its ```b2``` command,
* relies on build configuration existing in project *Jamfile* files
* relies on Boost.Build project hierarchy set up by *Jamroot* and *Jamfile* files.
* allows to specify ```b2``` *working directory* which can be different
 from both, *build directory* and *project directory*, where project directory means
 directory with Jamfile selected to open as a project.
* allows, thanks to previous features, to open only sub-project of a large project,
  for example, while working with Boost, one may use the Boost.Build plugin to compose
  a project which loads files only for selection of libraries, but request to run build
  against the whole Boost C++ libraries collection.

Finally, the Boost.Build plugin is a pilot project integrating Boost.Build system
with an IDE and this experience may be useful for future improvements and development
of Boost.Build system itself.

### What version of Qt Creator is supported?

I use Qt Creator 3.3, but it should work with Qt Creator 3.0 or later.
There are git tags corresponding to Qt Creator versions.

See also the [Installation](#installation) section.

### How to install the plugin?

[Build](#installation) it.

### Do I need to build Qt?

I don't know. Ask Qt folks.

I follow their
[Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html):

> Qt Creator requires private headers of Qt, which are unfortunately
> not installed by the Qt binary packages

### Do I need to build Qt Creator?

Apparently, yes, as per
[Getting and Building Qt Creator](http://doc-snapshot.qt-project.org/qtcreator-extending/getting-and-building.html):

> It is also necessary if you want to create your own Qt Creator plugin.

### Have you used it on Windows?

No.

### Have you used it on Mac OS X?

No.

### What is a *Jamfile*?

*Jamfile* is a makefile of Boost.Build written in dedicated
[Boost.Jam language](http://www.boost.org/boost-build2/doc/html/bbv2/overview/jam_language.html)
and interpreted by Boost.Build engine which is executed and controlled by
Boost.Build command ```b2```.
See [Boost.Build Overview](http://www.boost.org/boost-build2/doc/html/bbv2/overview.html)
for more details.

Boost.Build convention is that the top of
[project hierarchy](http://www.boost.org/boost-build2/doc/html/bbv2/tutorial/hierarchy.html)
is called project root where lives a *Jamfile* called ```Jamroot```.
A project may define subprojects and each subproject is defined by a *Jamfile*
called ```Jamfile``` in a descendant directory of the project root.

For both, root and subproject *Jamfiles*, Boost.Build recognises variety of file names:
* ```[Jj]amroot``` as well as ```[Jj]amroot.jam``` and ```project-root.jam```
* ```[Jj]amfile``` as well as ```[Jj]amfile.jam```, ```[Jj]amfile.v2" and  ```[Bb]uild.jam```.

Most of those names are recognised by the Boost.Build Project Manager plugin.

### What is ```*.qtcreator.files``` file for?

Qt Creator, the Boost.Build plugin, for a *Jamfile* which is opened as a project,
automatically generates ```.qtcreator.files``` file with list of all files selected
in the **Files Selection** page of the project wizard.

Role of this file is exactly the same as role of ```.files``` file for the
[Generic Project](http://doc-snapshot.qt-project.org/qtcreator-3.3/creator-project-generic.html).
It just uses different extension to clearly indicate its role and avoid file name clashes.

To add or remove files, edit the ```.qtcreator.files``` file in Qt Creator.
(**TODO:** Qt Creator recreates your project tree when you save the .files file.)

### What is ```*.qtcreator.includes``` file for?

Qt Creator, the Boost.Build plugin, for a *Jamfile* which is opened as a project,
automatically generates ```.qtcreator.includes``` file.

This file is dedicated to specify any include paths which are **solely** used by
Qt Creator for analysing C/C++ code of the project.
**Note**, these include paths do **not** affect compilation of source files whatsoever
as *Jamfile* files remain the only source of build configuration.

The project wizard generates initial list of include paths adding every directory
of project which contains C/C++ header files.
For large projects, like Boost, this may generate long list of includes paths,
though it should be easy to filter it.

To add or remove include paths, edit the ```.qtcreator.includes``` file in Qt Creator.
The paths can be either absolute or relative to the ```.qtcreator.includes``` file.

### What is ```*.user``` file for?

This file is natively created by Qt Creator when
[Opening Projects](http://doc-snapshot.qt-project.org/qtcreator-3.3/creator-project-opening.html):

> Qt Creator stores information that it needs to build projects in a .user file.

The Boost.Build Project Manager also uses this file to store information specific to
corresponding *Jamfile* project, like:
* name of *Jamfile* project which can be customised by user in the **Open Project** wizard
* path of working directory used to run Boost.Build command ```b2```
* path of build directory passed as value of ```b2``` option ```--build-dir```

### What is the Project Name in the Open Project wizard used for?


The **Open Boost.Build Project** wizard suggests project name on its first
page titled **Project Name and Paths**.

The wizard tries to extract project name from the Boost.Build *project* rule,
if present in the *Jamfile* being opened.
Otherwise, the wizard uses name of parent directory of the *Jamfile*.
Finally, user can change the **Project name** freely as it is only used by Qt Creator,
not by Boost.Build configuration.

The idea is to provide certain degree of freedom in arranging *Jamfile*-based projects
in Qt Creator session, regardless of their arrangements on disk.

This feature, for example, allows to generate and open multiple projects into Qt Creator
and all projects may refer to the same Boost library. For instance, working with
Boost.Geometry library, one may load  *Jamfile* for tests and *Jamfile* for examples,
selecting the same set of headers in both, and name the projects to reflect their purpose.

### What is the roadmap?

None, apart form plan to roll the project forward fixing bugs and adding features
as they appear useful or necessary in order to make work with Boost.Build-based
a bit more accessible and efficient.

See [Future](#future) section for some feature ideas.

### How can I contribute?

[Fork the repo](https://help.github.com/articles/fork-a-repo) and make the plugin better!


Future
------------------------------------------------------------------------------

An unprioritised TODO list for the plugin:

* Add tests! Add tests! Add tests! But first, learn how to test with Qt wealth!
* Add file watchers for Jamfile and sidecar files to automatically reload any changes.
    * Qt Creator recreates your project tree when you save the .files file.
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
* Parse Jamfile(s) or use b2 features if possible, to utomatically determine what
  executable(s) to run.

See also [opened enhancement issues](https://github.com/mloskot/qt-creator-plugin-boostbuild/issues?labels=enhancement&state=open)
at GitHub or search through the sources for ```TODO```, there is more items.
