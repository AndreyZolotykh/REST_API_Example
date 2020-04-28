-------------------------------------------------
#
# Project created by QtCreator 2019-12-24T13:45:38
#
#-------------------------------------------------

QT       -= gui
QT       += sql

TARGET = baseservice
TEMPLATE = lib
VERSION = 1.0.1

DEFINES += BASESERVICE_LIBRARY


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    oshs/oshsservice.cpp \
    var/tvar.cpp \
    baseservice.cpp \
    dbconnection.cpp \
    tests/speedtest.cpp \
    utils/utils.cpp \
    oshs/oshs.cpp \
    deployarea/deployareas.cpp \
    sign/sign.cpp \
    sign/signservice.cpp \
    sign/map.cpp \
    sign/mapservice.cpp \
    sign/layer.cpp \
    sign/layerservice.cpp \
    regrouping/march.cpp \
    regrouping/marchservice.cpp \
    vvt/attrvvt.cpp \
    vvt/attrvvtservice.cpp \
    regrouping/regroupingservice.cpp \
    regrouping/regroupingparampt.cpp

HEADERS += \
    oshs/oshsservice.h \
    var/tvar.h \
    baseservice.h \
    dbconnection.h \
    tests/speedtest.h \
    utils/utils.h \
    oshs/oshs.h \
    deployarea/deployareas.h \
    sign/sign.h \
    sign/signservice.h \
    sign/map.h \
    sign/mapservice.h \
    sign/layer.h \
    sign/layerservice.h \
    regrouping/march.h \
    regrouping/marchservice.h \
    vvt/attrvvt.h \
    vvt/attrvvtservice.h \
    regrouping/regroupingservice.h \
    regrouping/regroupingparampt.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

#unix:!macx: LIBS += -L$$/usr/local/lib -ldbconnect
OBJECTS_DIR = .tmp/obj
MOC_DIR = .tmp/moc
QMAKE_POST_LINK = "mkdir -p ../build" && "mv -f lib* ../build"
#QMAKE_POST_LINK = "mv -f lib* ../build"

#INCLUDEPATH += $$PWD/../dbconnect
#DEPENDPATH += $$PWD/../dbconnect

RESOURCES +=
