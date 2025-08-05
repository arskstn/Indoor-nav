// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow {
    objectName: "applicationWindow"
    initialPage: Qt.resolvedUrl(adapterStatus.adapterStatusPage)
    cover: Qt.resolvedUrl("cover/DefaultCoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Connections {
        target: adapterStatus
        onAdapterStatusPageChanged: {
            pageStack.replace(Qt.resolvedUrl(adapterStatus.adapterStatusPage))
        }
    }
}
