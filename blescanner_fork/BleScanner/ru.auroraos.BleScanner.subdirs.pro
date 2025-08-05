# SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
# SPDX-License-Identifier: BSD-3-Clause

TEMPLATE = subdirs

SUBDIRS = \
    app/ru.auroraos.BleScanner.pro \


OTHER_FILES += \
    AUTHORS.md \
    rpm/ru.auroraos.BleScanner.spec \
    CODE_OF_CONDUCT.md \
    CONTRIBUTING.md \
    LICENSE.BSD-3-CLAUSE.md \
    README.md \
    README.ru.md \

DISTFILES += \
    app/qml/pages/ErrorPage.qml \
    app/qml/pages/LoadPage.qml

RESOURCES += qml.qrc
