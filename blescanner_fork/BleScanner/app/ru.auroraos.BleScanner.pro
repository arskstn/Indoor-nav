# SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
# SPDX-License-Identifier: BSD-3-Clause

TEMPLATE = app

TARGET = ru.auroraos.BleScanner

QT += dbus network

PKGCONFIG += dbus-1

QMAKE_CXXFLAGS += -std=c++17

CONFIG += \
    auroraapp \

PKGCONFIG += \

INCLUDEPATH += \
    $$PWD/../libs/simpleble/include \
    $$PWD/../libs/simpledbus/include \
    $$PWD/../libs/simplebluez/include \
    $$OUT_PWD/../libs/simpleble \
    $$OUT_PWD/../libs/simpledbus \
    $$OUT_PWD/../libs/simplebluez \
    $$OUT_PWD/../libs/simpleble/export \
    /usr/include/dbus-1.0/dbus \

DEPENDPATH += \
    $$PWD/../libs/simpleble/include \
    $$PWD/../libs/simplebluez/include \
    $$PWD/../libs/simpledbus/include \
    $$OUT_PWD/../libs/simpleble \
    $$OUT_PWD/../libs/simpledbus \
    $$OUT_PWD/../libs/simplebluez \

LIBS += \
    -L$$OUT_PWD/../libs/simpleble \
    -lsimpleble \
    -lsimpleble-c \
    -L$$OUT_PWD/../libs/simpledbus \
    -lsimpledbus \
    -L$$OUT_PWD/../libs/simplebluez \
    -lsimplebluez \

SOURCES += \
    src/adaptercheck.cpp \
    src/blescanmodel.cpp \
    src/bluezscanmodel.cpp \
    src/main.cpp \
    src/peripheraladapter.cpp

HEADERS += \
    src/adaptercheck.h \
    src/blescanmodel.h \
    src/bluezscanmodel.h \
    src/peripheraladapter.h \
    src/basemodel.h

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.auroraos.BleScanner.ts \
    translations/ru.auroraos.BleScanner-ru.ts \

DISTFILES += \
    src/IMUFusion.qml \
    src/RSSIFingerprinting.qml

RESOURCES += \
    qml.qrc
