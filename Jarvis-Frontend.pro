#-------------------------------------------------
#
# Project created by QtCreator 2012-08-08T21:48:47
#
#-------------------------------------------------

QT       += core network
QT -= gui

TARGET = Jarvis-Frontend
TEMPLATE = lib
debug: DESTDIR = $$PWD/debug
release: DESTDIR = $$PWD/release
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    Room.cpp \
    ModulePackage.cpp \
    Module.cpp \
    JarvisClient.cpp \
    FunctionModule.cpp \
    OperatorModule.cpp \
    BinaryOperatorModule.cpp \
    UnaryOperatorModule.cpp

HEADERS  += \
    Room.h \
    ModulePackage.h \
    Module.h \
    JarvisClient.h \
    OperatorModule.h \
    FunctionModule.h \
    BinaryOperatorModule.h \
    UnaryOperatorModule.h \
    FunctionSignature.h

OTHER_FILES += \
    README.md \
    .gitignore
