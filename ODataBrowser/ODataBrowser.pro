#-------------------------------------------------
#
# Project created by QtCreator 2017-05-16T18:47:24
#
#-------------------------------------------------

QT     += core gui

CONFIG += static
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

INCLUDEPATH += \
    ../Lib \
    ../Lib/OakXML \
    ../Lib/OakModel \
    ../Lib/OakView

include(NodeDef.pri)


DEFINES += XML_BACKEND
DEFINES += NOMINMAX

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui


CONFIG(debug, debug|release) {
    TARGET = ODataBrowserd
    win32:POST_TARGETDEPS = \
        ../Lib/OakXMLd.lib \
        ../Lib/OakModeld.lib \
        ../Lib/OakViewd.lib
    win32:LIBS = \
        ../Lib/OakXMLd.lib \
        ../Lib/OakModeld.lib \
        ../Lib/OakViewd.lib
    OBJECTS_DIR = ./debug

} else {
    TARGET = ODataBrowser
    win32:POST_TARGETDEPS = \
        ../Lib/OakXML.lib \
        ../Lib/OakModel.lib \
        ../Lib/OakView.lib
    win32:LIBS = \
        ../Lib/OakXML.lib \
        ../Lib/OakModel.lib \
        ../Lib/OakView.lib
    OBJECTS_DIR = ./release
}

RESOURCES += \
    Resources.qrc