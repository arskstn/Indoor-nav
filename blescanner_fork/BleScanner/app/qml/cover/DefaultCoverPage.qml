// SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    objectName: "defaultCover"

    CoverPlaceholder {
        objectName: "placeholder"
        text: qsTr("BLE Scanner")
        icon {
            source: Qt.resolvedUrl("../icons/app.svg")
            sourceSize { width: icon.width; height: icon.height }
        }
        forceFit: true
    }
}
