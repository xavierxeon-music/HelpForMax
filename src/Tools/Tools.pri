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
   $$PWD/Lock.h \
   $$PWD/ModelItem.h \
   $$PWD/Settings.h \
   $$PWD/TypeDelegate.h

SOURCES += \
   $$PWD/AbstractItemTreeView.cpp \
   $$PWD/Central.cpp \
   $$PWD/JSONModel.cpp \
   $$PWD/Lock.cpp \
   $$PWD/ModelItem.cpp \
   $$PWD/Settings.cpp \
   $$PWD/TypeDelegate.cpp
