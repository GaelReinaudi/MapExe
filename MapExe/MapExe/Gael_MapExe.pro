# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = MapExe
DESTDIR = ../../bin
QT += core gui
CONFIG += debug
INCLUDEPATH += ./../../../LabExe/src \
    ./../../../MapExe/src \
    ./../../../MapExe/include \
    ./GeneratedFiles \
    ./GeneratedFiles/Debug \
    .
LIBS += -L"./../../../MapExe/lib" \
    -L"./../../../MapExe/bin" \
    -L"." \
    -llabexe_D \
    -lLabExeGraphicsMap_D
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(MapExe.pri)