TEMPLATE = app
TARGET = popcc
CONFIG -= qt app_bundle
INCLUDEPATH += $$PWD/..
SOURCES = ccomp.cpp
LIBS += -L$$OUT_PWD/../pop/ -lpop
DEPENDPATH += $$PWD/../pop
QMAKE_CXXFLAGS += -std=c++14
