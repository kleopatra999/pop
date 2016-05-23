TEMPLATE = app
TARGET = popdis
CONFIG -= qt app_bundle
INCLUDEPATH += $$PWD/..
SOURCES = dis.cpp
LIBS += -L$$OUT_PWD/../pop/ -lpop
DEPENDPATH += $$PWD/../pop
QMAKE_CXXFLAGS += -std=c++14
