// SPDX-FileCopyrightText: Copyright 2025 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.omp.SimpleBLE 1.0

Page {
    objectName: "rssiFingerprintingPage"
    allowedOrientations: Orientation.All

    PageHeader {
        id: pageHeader
        title: qsTr("RSSI Fingerprinting")
    }

    Column {
        anchors {
            top: pageHeader.bottom
            left: parent.left
            right: parent.right
            margins: Theme.paddingLarge
        }
        spacing: Theme.paddingMedium

        Label {
            text: qsTr("RSSI Fingerprinting Analysis")
            font.pixelSize: Theme.fontSizeLarge
            color: Theme.primaryColor
        }

        Label {
            text: qsTr("Use this page to analyze RSSI data for fingerprinting.")
            font.pixelSize: Theme.fontSizeMedium
            color: Theme.secondaryColor
        }

        Button {
            text: qsTr("Start Analysis")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: console.log("Start RSSI analysis")
        }

        Button {
            text: qsTr("Save Data")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: console.log("Save RSSI data")
        }

        Image {
            width: parent.width
            height: Theme.itemSizeExtraLarge * 12
            source: "qrc:/icons/kartasmall.png"
            fillMode: Image.Stretch
        }
    }
}
