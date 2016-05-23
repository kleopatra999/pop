TEMPLATE = app
TARGET = poptok
CONFIG -= qt app_bundle
INCLUDEPATH += $$PWD/..
SOURCES = tokdump.cpp
LIBS += -L$$OUT_PWD/../pop/ -lpop
DEPENDPATH += $$PWD/../pop
QMAKE_CXXFLAGS += -std=c++14
