#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T15:34:14
#
#-------------------------------------------------
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = bedcoll
QMAKE_CC = clang
QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -D_FILE_OFFSET_BITS=64
INCLUDEPATH += /usr/include/c++/4.8
INCLUDEPATH += /usr/include/boost
#static_libs_path = "-L/usr/lib/x86_64-linux-gnu"
#LIBS += $$static_libs_path/libboost_system.a
#LIBS += $$static_libs_path/libboost_filesystem.a
#LIBS += $$static_libs_path/libboost_program_options.a
LIBS += -lboost_system
LIBS += -lboost_filesystem
LIBS += -lboost_program_options


SOURCES += \
    src/util.cpp \
    src/bedcoll.cpp \
    src/main.cpp

HEADERS += \
    src/collgen_memo.h \
    src/util.h \
    src/bedcoll.h \
    src/error_enum.h
