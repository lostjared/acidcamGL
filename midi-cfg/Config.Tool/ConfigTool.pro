######################################################################
# Automatically generated by qmake (3.1) Mon Jun 24 11:54:27 2024
######################################################################
QT += core gui widgets
TEMPLATE = app
TARGET = ConfigTool
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += mainwindow.h RtMidi.h
SOURCES += main.cpp mainwindow.cpp
LIBS += `pkg-config rtmidi --libs`
