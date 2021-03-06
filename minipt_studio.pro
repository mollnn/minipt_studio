#-------------------------------------------------
#
# Project created by QtCreator 2021-05-09T22:38:36
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minipt_studio
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        main.cpp \
        glwidget.cpp \
        camera.cpp \
        # minipt module
        ./minipt/color.cpp \
        ./minipt/image.cpp \
        ./minipt/minipt.cpp \
        ./minipt/pathtracer.cpp \
        ./minipt/random.cpp \
        ./minipt/renderer.cpp \
        ./minipt/testscene.cpp \
        ./minipt/tgaimage.cpp \
        ./minipt/timer.cpp \
        ./minipt/triangle.cpp \
        ./minipt/vec3.cpp \
    cmdedit.cpp

HEADERS += \
        glwidget.h \
    cmdedit.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
