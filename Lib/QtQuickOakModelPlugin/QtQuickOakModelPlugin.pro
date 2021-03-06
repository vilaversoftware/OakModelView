TEMPLATE = lib
CONFIG += plugin
CONFIG += c++1z
QT += qml quick

TARGET = oakmodelplugin

DEFINES += XML_BACKEND
DEFINES += NOMINMAX

DESTDIR += ../Plugins/Oak/Model

INCLUDEPATH += \
    . \
    .. \
    ../OakXML \
    ../OakModel \
    ../QtOakModel

	
HEADERS += \
    oakmodelplugin.h


SOURCES += \
    oakmodelplugin.cpp

# Make the qmldir file visible in the Project tree
OTHER_FILES += qmldir

# Copy the qmldir file to the same folder as the plugin binary
cpqmldir.files = qmldir
cpqmldir.path = $$DESTDIR
COPIES += cpqmldir


TARGET = $$qtLibraryTarget($$TARGET)

CONFIG(debug, debug|release) {
    win32:POST_TARGETDEPS = \
        ../OakXMLd.lib \
        ../OakModeld.lib \
        ../QtOakModeld.lib
    win32:LIBS = \
        ../OakXMLd.lib \
        ../OakModeld.lib \
        ../QtOakModeld.lib
    OBJECTS_DIR = ./debug
} else {
    win32:POST_TARGETDEPS = \
        ../OakXML.lib \
        ../OakModel.lib \
        ../QtOakModel.lib
    win32:LIBS = \
        ../OakXML.lib \
        ../OakModel.lib \
        ../QtOakModel.lib
    OBJECTS_DIR = ./release
}

##### Stuff needed by the installer #####

#DESTPATH = ../../installer

#target.files += files.qml
#target.path = $$DESTPATH

#qmldir.files = $$PWD/qmldir
#qmldir.path  =$$DESTPATH

#INSTALLS += target qmldir

#CONFIG += install_ok  # Do not cargo-cult this!

