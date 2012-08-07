#-------------------------------------------------
#
# Project created by QtCreator 2012-07-31T11:54:06
#
#-------------------------------------------------

QT       += core network

QT       -= gui
QMAKE_CXXFLAGS += -std=c++11
TARGET = console-client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../JarvisClient.cpp \
    TerminalPrinter.cpp \
    ModulePackage.cpp \
    Module.cpp \
    Scope.cpp

HEADERS += \
    ../JarvisClient.h \
    TerminalPrinter.h \
    InputWorker.h \
    ModulePackage.h \
    Module.h \
    Scope.h
