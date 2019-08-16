TEMPLATE = app
TARGET = gltest
DEPENDPATH += . src
INCLUDEPATH += . src 

QT += core
QT += gui
QT += opengl

OBJECTS_DIR=.obj
MOC_DIR=.moc

CONFIG += opengl

win32: LIBS += -lopengl32
win32: DEFINES += _USE_MATH_DEFINES
win32: DEFINES -= UNICODE

unix: LIBS += -lX11

OTHER_FILES += \
    data/face.obj \

# Input
HEADERS += \
    src/glext.h \
    src/glwindow.h \
    src/gldevice.h \
    src/shader.h \
    src/mesh.h \
    src/subdivision.h \
    src/objloader.h

SOURCES += \
    src/main.cpp \
    src/glwindow.cpp \
    src/gldevice.cpp \
    src/mesh.cpp \
    src/subdivision.cpp \
    src/objloader.cpp

HEADERS += \
    src/vector3.h \
    src/vector2.h \

HEADERS += \
    src/array.h \
    src/referenced.h \
    src/singleton.h \

SOURCES += \
    src/referenced.cpp \
