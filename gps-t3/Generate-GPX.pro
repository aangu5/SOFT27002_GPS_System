TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wfatal-errors

HEADERS += \
    headers/earth.h \
    headers/geometry.h \
    headers/logs.h \
    headers/position.h \
    headers/types.h \
    headers/gridworld/gridworld_model.h \
    headers/gridworld/gridworld_route.h \
    headers/gridworld/gridworld_track.h \
    headers/xml/generator.h \


SOURCES += \
    src/earth.cpp \
    src/geometry.cpp \
    src/logs.cpp \
    src/position.cpp \
    src/gridworld/gridworld_model.cpp \
    src/gridworld/gridworld_route.cpp \
    src/gridworld/gridworld_track.cpp \
    src/gridworld/generate-gpx.cpp \
    src/xml/generator.cpp


INCLUDEPATH += headers/  headers/gridworld/  headers/xml/

TARGET = $$_PRO_FILE_PWD_/execs/generate-gpx
