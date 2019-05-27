QT       += widgets

TEMPLATE = lib
CONFIG += staticlib

include(../Configure.pri)

INCLUDEPATH += \
    . \
    .. \
    ../OakXML \
    ../OakModel

SOURCES += \
    ValueEditorHandler.cpp \
    NodeEditorHandler.cpp \
    ParentDataView.cpp \
    ContainerEditorHandler.cpp \
    ContainerEditor.cpp \
    ServiceFunctions.cpp \
    NodeDataView.cpp \
    OakView.cpp \
    ListView.cpp \
    TableView.cpp \
    ActionToolBar.cpp \
    ListViewNode.cpp

HEADERS += \
    ValueEditorHandler.h \
    NodeEditorHandler.h \
    ParentDataView.h \
    ContainerEditorHandler.h \
    ContainerEditor.h \
    ServiceFunctions.h \
    NodeDataView.h \
    OakView.h \
    ListView.h \
    TableView.h \
    ActionToolBar.h \
    ListViewNode.h


CONFIG(debug, debug|release) {
    TARGET = ../../QtWidgetOakViewd
    OBJECTS_DIR = ./debug
} else {
    TARGET = ../../QtWidgetOakView
    OBJECTS_DIR = ./release
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    OakViewResources.qrc
