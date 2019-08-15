#-------------------------------------------------
#
# Project created by QtCreator 2019-08-02T15:33:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyTreeView
TEMPLATE = app
CONFIG += c++11
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lWLVideoPlayer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lWLVideoPlayerd
else:unix: LIBS += -L$$PWD/lib/ -lWLVideoPlayer

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib
SOURCES += main.cpp\
        mainwindow.cpp \
    treemodel.cpp \
    treeitem.cpp \
    videotreewidget.cpp

HEADERS  += mainwindow.h \
    treemodel.h \
    treeitem.h \
    videotreewidget.h

FORMS    += mainwindow.ui
