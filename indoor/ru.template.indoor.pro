TARGET = ru.template.indoor

CONFIG += \
    auroraapp

PKGCONFIG += \

SOURCES += \
    src/blescanner.cpp \
    src/main.cpp \

HEADERS += \
    src/blescanner.h

DISTFILES += \
    rpm/ru.template.indoor.spec \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.indoor.ts \
    translations/ru.template.indoor-ru.ts \

QT += bluetooth network
