#-------------------------------------------------
#
# Project created by QtCreator 2012-08-08T21:48:47
#
#-------------------------------------------------

QT       += core network
QT -= gui

TARGET = Jarvis-Frontend
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    Scope.cpp \
    ModulePackage.cpp \
    Module.cpp \
    JarvisClient.cpp

HEADERS  += \
    Scope.h \
    ModulePackage.h \
    Module.h \
    JarvisClient.h

OTHER_FILES += \
    README.md \
    .gitignore
