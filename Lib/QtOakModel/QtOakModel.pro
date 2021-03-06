TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++1z

DEFINES += XML_BACKEND
DEFINES += NOMINMAX

INCLUDEPATH += \
    . \
    .. \
    ../OakXML \
    ../OakModel

include(ToolBar.pri)

HEADERS += \
    QContainerDefBuilderData.h \
    QOakAbstractNodeModel.h \
    QPropertyHelpers.h \
    QLeafDefBuilderData.h \
    QLeafSettingsBuilderData.h \
    QNodeDefBuilderData.h \
    QOakBaseTypes.h \
    QServiceFunctions.h \
    QValueOptionsBuilderData.h \
    QOakModel.h \
    QOakModelBuilderData.h \
    QOakNodeProxyModel.h


SOURCES += \
    QContainerDefBuilderData.cpp \
    QLeafDefBuilderData.cpp \
    QLeafSettingsBuilderData.cpp \
    QNodeDefBuilderData.cpp \
    QOakAbstractNodeModel.cpp \
    QServiceFunctions.cpp \
    QValueOptionsBuilderData.cpp \
    QOakModel.cpp \
    QOakModelBuilderData.cpp \
    QOakNodeProxyModel.cpp

CONFIG(debug, debug|release) {
    TARGET = ../../QtOakModeld
    win32:POST_TARGETDEPS = \
        ../OakXMLd.lib \
        ../OakModeld.lib
    win32:LIBS = \
        ../OakXMLd.lib \
        ../OakModeld.lib
    OBJECTS_DIR = ./debug
} else {
    TARGET = ../../QtOakModel
    win32:POST_TARGETDEPS = \
        ../OakXML.lib \
        ../OakModel.lib
    win32:LIBS = \
        ../OakXML.lib \
        ../OakModel.lib
    OBJECTS_DIR = ./release
}


