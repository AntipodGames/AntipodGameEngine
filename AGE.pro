LIBS +=  -lX11  -L/usr/local/lib  -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system  -L/usr/lib/x86_64-linux_gnu/ -lyaml-cpp
#-lGL -lGLU

INCLUDEPATH += /usr/local/include include
# /usr/include/boost

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
QT += gui
QT += core
QT += widgets

CONFIG += console
CONFIG -= app_bundle

SOURCES += \
    src/engine.cpp \
    src/animatedmanager.cpp \
    src/animatedsprite.cpp \
    src/texturemanager.cpp \
    src/displaymanager.cpp \
    src/system.cpp \
    src/collider.cpp

HEADERS += \
    include/AGE/parameters.hpp \
    include/AGE/system.hpp \
    include/AGE/engine.hpp \
    include/AGE/entity.hpp \
    include/AGE/texturemanager.h \
    include/AGE/animatedmanager.h \
    include/AGE/animatedsprite.h \
    include/AGE/displaymanager.h \
    include/AGE/types.hpp \
    include/AGE/qsystem.hpp \
    include/AGE/collider.hpp \
    include/AGE/quadtree.hpp

lib_mode {
TEMPLATE = lib
TARGET = AGE
target = HEADERS libage
target.path = /usr/local
INSTALLS += target

}

test_mode {
TEMPLATE = app
SOURCES += test/test_1/core.cpp
HEADERS += test/test_1/core.hpp test/test_1/scripts.hpp
TARGET = test_1

}
