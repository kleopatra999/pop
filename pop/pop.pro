TEMPLATE = app
TARGET = pop
CONFIG -= qt app_bundle
QMAKE_CXXFLAGS = -std=c++14 -Wno-unused-parameter
INCLUDEPATH += $$PWD/..
#DEFINES += POP_COMPILING=1
SOURCES = main.cpp
LIBS += -L$$OUT_PWD/ -lpop
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
