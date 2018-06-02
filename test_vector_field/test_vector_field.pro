TEMPLATE = app
CONFIG += c++11
QT += gui
QT += core
QT += widgets

INCLUDEPATH += /usr/local/include ../../include
# /usr/include/boost
LIBS +=  -lX11
LIBS += -L/usr/local/lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -L/usr/lib/x86_64-linux_gnu/ -lyaml-cpp
LIBS += -L../../build/debug/ -lAGE

SOURCES += \
    core.cpp

HEADERS += \
    core.hpp \
    scripts.hpp
