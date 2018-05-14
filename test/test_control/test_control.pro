QMAKE_CXXFLAGS += -std=c++11 -O0
CONFIG += c++11
QT += gui
QT += core
QT += widgets


TEMPLATE = app
SOURCES += core.cpp player.cpp
HEADERS += core.h player.h
TARGET = test_control
LIBS +=  -lX11
LIBS += -L/usr/local/lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -L/usr/lib/x86_64-linux_gnu/ -lyaml-cpp
LIBS += -L../../build/debug/ -lAGE
#-lGL -lGLU


INCLUDEPATH += /usr/local/include ../../include
#/usr/include/boost
