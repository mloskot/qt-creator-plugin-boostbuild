#
# Copyright (C) 2013 Mateusz Łoskot <mateusz@loskot.net>
#
# This file is part of Qt Creator Boost.Build plugin project.
#
# This is free software; you can redistribute and/or modify it under
# the terms of the GNU Lesser General Public Licence as published
# by the Free Software Foundation.
# See the LICENSE.txt file for more information.
#
QTC_PLUGIN_NAME = BoostBuildProjectManager

PROVIDER = mloskot

DEFINES += BOOSTBUILDPROJECTMANAGER_LIBRARY

CONFIG(debug, release|debug):DEFINES += _DEBUG
CONFIG += c++11

SOURCES += \
    bbbuildconfiguration.cpp \
    bbbuildinfo.cpp \
    bbbuildstep.cpp \
    bbopenprojectwizard.cpp \
    bboutputparser.cpp \
    bbproject.cpp \
    bbprojectfile.cpp \
    bbprojectmanager.cpp \
    bbprojectmanagerplugin.cpp \
    bbprojectnode.cpp \
    bbutility.cpp \
    filesselectionwizardpage.cpp \
    selectablefilesmodel.cpp \
    external/projectexplorer/clangparser.cpp \
    external/projectexplorer/gccparser.cpp \
    external/projectexplorer/ldparser.cpp

HEADERS += \
    bbbuildconfiguration.hpp \
    bbbuildinfo.hpp \
    bbbuildstep.hpp \
    bbopenprojectwizard.hpp \
    bboutputparser.hpp \
    bbproject.hpp \
    bbprojectfile.hpp \
    bbprojectmanager.hpp \
    bbprojectmanager_global.hpp \
    bbprojectmanagerconstants.hpp \
    bbprojectmanagerplugin.hpp \
    bbprojectnode.hpp \
    bbutility.hpp \
    filesselectionwizardpage.hpp \
    selectablefilesmodel.hpp \
    external/projectexplorer/clangparser.h \
    external/projectexplorer/gccparser.h \
    external/projectexplorer/ldparser.h

RESOURCES += \
    boostbuildproject.qrc

OTHER_FILES += \
    $$QTC_PLUGIN_NAME.json \
    LICENSE.LGPL \
    NOTES.md

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/mloskot/dev/qt/_git/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/mloskot/dev/qt/_git/qt-creator/_build

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.


QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    projectexplorer \
    qtsupport

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

