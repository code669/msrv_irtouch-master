TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    msrv_irtouch.cpp

HEADERS += \
    msrv_irtouch.h \
    ffprotocol_info.h \
    commands_def.h \
    hid-tp-coords-ops.h

