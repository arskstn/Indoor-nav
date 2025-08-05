// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    BusyIndicator {
        id: busyIndicator

        anchors.centerIn: parent
        running: true
    }
    Label {
        anchors.top: busyIndicator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Checking the adapter status...")
    }
}
