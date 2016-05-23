TEMPLATE = app
TARGET = popast
CONFIG -= qt app_bundle
INCLUDEPATH += $$PWD/..
SOURCES = astdump.cpp
LIBS += -L$$OUT_PWD/../pop/ -lpop
DEPENDPATH += $$PWD/../pop
QMAKE_CXXFLAGS += -std=c++14
