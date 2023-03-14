TARGET = HelpForMax
TEMPLATE = app

QT += widgets xml svg
CONFIG += c++20

DESTDIR = ../bin

macx {
    ICON = $$PWD/icons/H4M.icns
    CONFIG(release, debug|release): DESTDIR = ~/Applications
}

windows {
    RC_ICONS = $$PWD/icons/H4M.ico
}

include(Edit/Edit.pri)
include(Tools/Tools.pri)

HEADERS += \
   ComponentsModel.h \
   ComponentsView.h \
   EditWidget.h \
   MainWindow.h \
   Message.h \
   PatchParser.h \
   SelectModel.h \
   SelectView.h

SOURCES += \
   ComponentsModel.cpp \
   ComponentsView.cpp \
   EditWidget.cpp \
   MainWindow.cpp \
   Message.cpp \
   PatchParser.cpp \
   SelectModel.cpp \
   SelectView.cpp

RESOURCES += \
   icons/icons.qrc
