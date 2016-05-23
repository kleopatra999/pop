TEMPLATE = app
TARGET = popc
CONFIG -= qt app_bundle
INCLUDEPATH += $$PWD/..
SOURCES = comp.cpp
LIBS += -L$$OUT_PWD/../pop/ -lpop
DEPENDPATH += $$PWD/../pop
QMAKE_CXXFLAGS += -std=c++14
