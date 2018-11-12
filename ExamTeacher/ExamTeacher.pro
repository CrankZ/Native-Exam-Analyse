#-------------------------------------------------
#
# Project created by QtCreator 2017-10-14T11:43:12
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += axcontainer
#xlsx用于生成表格。非官方组件，具体请看
#https://github.com/dbzhang800/QtXlsxWriter
QT       += xlsx

INCLUDEPATH += $$PWD/QuaZIP/include
LIBS += -L$$PWD/QuaZIP/lib -lquazip -L$$PWD/zlib128-dll/ -lzlib1

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExamTeacher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    setpaper.cpp \
    viewpaper.cpp \
    sender.cpp \
    receiver.cpp \
    typeaddwidget.cpp \
    analyzer.cpp \
    singleapplication.cpp

HEADERS += \
        widget.h \
    setpaper.h \
    viewpaper.h \
    justenum.h \
    sender.h \
    receiver.h \
    typeaddwidget.h \
    examinfo.h \
    analyzer.h \
    singleapplication.h

FORMS += \
        widget.ui \
    setpaper.ui \
    viewpaper.ui \
    typeaddwidget.ui

RESOURCES += \
    resource.qrc

RC_FILE = rc.rc
