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
include(Tools/Tools.pri)

include(Select/01_Select.pri)
include(Overview/02_Overview.pri)
include(Component/03_Component.pri)
include(Edit/04_Edit.pri)
include(Result/05_Result.pri)

HEADERS += \
   MainWindow.h

SOURCES += \
   MainWindow.cpp

RESOURCES += \
   icons/icons.qrc
