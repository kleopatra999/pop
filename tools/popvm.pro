TEMPLATE = app
TARGET = popvm
CONFIG -= qt app_bundle
INCLUDEPATH += $$PWD/..
SOURCES = vm.cpp
LIBS += -L$$OUT_PWD/../pop/ -lpop
DEPENDPATH += $$PWD/../pop
QMAKE_CXXFLAGS += -std=c++14
