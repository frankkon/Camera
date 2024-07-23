QT -= gui
QT += httpserver sql

TEMPLATE = app
TARGET = camera

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += debug_and_release
CONFIG += build_all

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
} else {
    DEFINES += QT_NO_DEBUG_OUTPUT
    TARGET = $$TARGET
}

DESTDIR = ../bin

win32 {
INCLUDEPATH += ../include \
            ../include/mvs \
            ../include/gphoto2 \
            ../include/sqlite3

LIBS += -L"../lib/mvs" -lMvCameraControl
LIBS += -L"../lib/gphoto2" -lgphoto2
LIBS += -L"../lib/sqlite3" -lsqlite3
}

unix {
DEFINES += LINUX_DEAMON

INCLUDEPATH += /opt/MVS/include

LIBS += -L"/opt/MVS/lib/64" -lMvCameraControl
LIBS += -lgphoto2 -lsqlite3
}

SOURCES += \
        camera.cpp \
        config.cpp \
        digitalcamera.cpp \
        global.cpp \
        industorycameraconfig.cpp \
        industrycamera.cpp \
        log.cpp \
        main.cpp \
        service.cpp \
        srvconfig.cpp \
        system.cpp

HEADERS += \
    camera.h \
    config.h \
    constdef.h \
    digitalcamera.h \
    global.h \
    industorycameraconfig.h \
    industrycamera.h \
    log.h \
    service.h \
    srvconfig.h \
    system.h

