TEMPLATE = lib

LIBS +=  -lX11  -lsfml-window -lsfml-system -lsfml-graphics -lsfml-audio -L/usr/lib/x86_64-linux_gnu/ -lyaml-cpp
#-lGL -lGLU

INCLUDEPATH += /usr/local/include/SFML include
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
    src/circlecollider.cpp \
    src/animatedmanager.cpp \
    src/animatedsprite.cpp \
    src/texturemanager.cpp \
    src/displaymanager.cpp

HEADERS += \
    include/AGE/parameters.hpp \
    include/AGE/system.hpp \
    include/AGE/engine.hpp \
    include/AGE/entity.hpp \
    include/AGE/circlecollider.hpp \
    include/AGE/texturemanager.h \
    include/AGE/animatedmanager.h \
    include/AGE/animatedsprite.h \
    include/AGE/displaymanager.h \
    include/AGE/types.hpp

target = HEADERS libage
target.path = /usr/local
INSTALLS += target
