// SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    property var characteristicList:[]

    objectName: "aboutService"
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView

        anchors.fill: parent
        model: characteristicList
        spacing: Theme.paddingMedium
        header: PageHeader {
            width: listView.width
            title: qsTr("Characteristic list")
        }
        delegate: ListItem {
            height: characteristicUUID.height
            contentHeight: height
            Label{
                id: characteristicUUID

                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.Wrap
                text: qsTr("UUID: ") + modelData
            }
        }
    }
}
