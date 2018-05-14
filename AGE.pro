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
    include/AGE/quadtree.hpp \
    include/AGE/controller.hpp

lib_mode {
TEMPLATE = lib

LIBS +=  -lX11
LIBS += -L/usr/local/lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -L/usr/lib/x86_64-linux_gnu/ -lyaml-cpp
#-lGL -lGLU

INCLUDEPATH += /usr/local/include include
# /usr/include/boost

QMAKE_CXXFLAGS += -std=c++11 -O0
CONFIG += c++11
QT += core
QT += gui
QT += widgets


CONFIG += console
CONFIG -= app_bundle
CONFIG += ordered


TARGET = AGE

target = HEADERS libage
target.path = /usr/local
INSTALLS += target

}

test_mode {
TEMPLATE = subdirs
test_1.subdir = test/test_1
test_1.target = test_1
test_control.subdir = test/test_control
test_control.target = test_control

SUBDIRS = test_1 test_control

}

