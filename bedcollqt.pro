#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T15:34:14
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = bedcoll
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CC = clang
QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -D_FILE_OFFSET_BITS=64
INCLUDEPATH += /usr/include/c++/4.8
INCLUDEPATH += /usr/include/boost
LIBS += -lboost_system
LIBS += -lboost_filesystem
LIBS += -lboost_program_options

TEMPLATE = app


SOURCES += \
    src/util.cpp \
    src/bedcoll.cpp \
    src/main.cpp

HEADERS += \
    src/collgen_memo.h \
    src/util.h \
    src/bedcoll.h \
    src/error_enum.h
