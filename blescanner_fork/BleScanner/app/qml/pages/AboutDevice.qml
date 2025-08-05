// SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    property var serviceList

    objectName: "aboutDevice"
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: serviceList
        spacing: Theme.paddingMedium
        header: PageHeader {
            width: listView.width
            title: qsTr("Services list")
        }
        delegate: ListItem {
            height: uuidLabel.height +
                    validLabel.height +
                    pathLabel.height +
                    (Theme.paddingSmall * 3)
            contentHeight: height

            Label {
                id: uuidLabel

                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.Wrap
                text: qsTr("UUID: ") + modelData.uuid
            }
            Label {
                id: validLabel

                anchors {
                    top: uuidLabel.bottom
                    left: parent.left
                    right: parent.right
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }
                text: qsTr("Valid: ") + modelData.valid
            }
            Label {
                id: pathLabel

                anchors {
                    top: validLabel.bottom
                    left: parent.left
                    right: parent.right
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.Wrap
                text: qsTr("Path: ") + modelData.path
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("AboutService.qml"),
                               {characteristicList: modelData.introspectCharacterisitics()})
            }
        }
    }
}
