#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T14:53:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GoogleProtocolBuffer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    lib/include/google/protobuf/compiler/importer.h \
    lib/include/google/protobuf/dynamic_message.h

FORMS    += mainwindow.ui

win32: LIBS += -L$$PWD/lib/ -lprotobuf

INCLUDEPATH += $$PWD/lib/include
DEPENDPATH += $$PWD/lib/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/protobuf.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/libprotobuf.a
