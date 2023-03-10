INCLUDEPATH += $$PWD

macx {
    HEADERS += $$PWD/MacTheme.h
    OBJECTIVE_SOURCES += $$PWD/MacTheme.mm
}


HEADERS += \
   $$PWD/AbstractFunctionHub.h \
   $$PWD/AbstractFunctionHub.hpp \
   $$PWD/AbstractItemTreeView.h \
   $$PWD/AbstractItemTreeView.hpp \
   $$PWD/Central.h \
   $$PWD/JSONModel.h \
   $$PWD/Message.h \
   $$PWD/ModelItem.h \
   $$PWD/PatchStructure.h \
   $$PWD/Settings.h \
   $$PWD/SpacerToolBar.h \
   $$PWD/TypeDelegate.h

SOURCES += \
   $$PWD/AbstractItemTreeView.cpp \
   $$PWD/Central.cpp \
   $$PWD/JSONModel.cpp \
   $$PWD/Message.cpp \
   $$PWD/ModelItem.cpp \
   $$PWD/PatchStructure.cpp \
   $$PWD/Settings.cpp \
   $$PWD/SpacerToolBar.cpp \
   $$PWD/TypeDelegate.cpp
