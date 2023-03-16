TARGET = HelpForMax
TEMPLATE = app

QT += widgets xml svg
CONFIG += c++20

DESTDIR = ../bin

macx {
    ICON = $$PWD/icons/HelpForMax.icns
    CONFIG(release, debug|release): DESTDIR = ~/Applications
}

windows {
    RC_ICONS = $$PWD/icons/HelpForMax.ico
}

include(Edit/Edit.pri)
include(Tools/Tools.pri)

HEADERS += \
   ComponentsModel.h \
   ComponentsView.h \
   EditWidget.h \
   MainWindow.h \
   PatchParser.h \
   SelectModel.h \
   SelectView.h

SOURCES += \
   ComponentsModel.cpp \
   ComponentsView.cpp \
   EditWidget.cpp \
   MainWindow.cpp \
   PatchParser.cpp \
   SelectModel.cpp \
   SelectView.cpp

RESOURCES += \
   icons/icons.qrc
