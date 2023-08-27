TARGET = HelpForMax
TEMPLATE = app

QT += widgets xml svg
CONFIG += c++20

DESTDIR = ../bin

macx {
   QMAKE_CXXFLAGS += -Werror
   ICON = $$PWD/icons/HelpForMax.icns
   CONFIG(release, debug|release): DESTDIR = ~/Applications
}

windows {
   QMAKE_CXXFLAGS += /WX
   RC_ICONS = $$PWD/icons/HelpForMax.ico
}

include(Block/Block.pri)
include(Clean/Clean.pri)
include(Component/Component.pri)
include(Edit/Edit.pri)
include(Preview/Preview.pri)
include(Select/Select.pri)
include(Tools/Tools.pri)

HEADERS += \
   MainWindow.h

SOURCES += \
   MainWindow.cpp

RESOURCES += \
   icons/icons.qrc
