TEMPLATE = app
TARGET = test_lexer
CONFIG -= qt app_bundle
QMAKE_CXXFLAGS += -std=c++14
INCLUDEPATH += $$PWD/..
LIBS += -L$$OUT_PWD/../pop/ -lpop
DEPENDPATH += $$PWD/../pop
SOURCES = test_lexer.cpp
