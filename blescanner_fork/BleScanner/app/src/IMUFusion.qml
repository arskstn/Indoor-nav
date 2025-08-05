// SPDX-FileCopyrightText: Copyright 2025 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.omp.SimpleBLE 1.0

Page {
    objectName: "imuFusionPage"
    allowedOrientations: Orientation.All

    PageHeader {
        id: pageHeader
        title: qsTr("IMU Fusion")
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
            text: qsTr("IMU Data Analysis")
            font.pixelSize: Theme.fontSizeLarge
            color: Theme.primaryColor
        }

        // Placeholder for IMU data
        TextArea {
            id: imuDataArea
            width: parent.width
            height: Theme.itemSizeExtraLarge
            readOnly: true
            text: qsTr("IMU data will be displayed here (accelerometer, gyroscope, etc.)")
            label: qsTr("IMU Data")
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        TextSwitch {
            id: sortSwitch
            text: qsTr("Sort by Name")
            description: qsTr("Sort discovered devices alphabetically by name")
            onCheckedChanged: {
                bleScanModel.sortByName(checked)
            }
        }

        // BLE devices list
        SilicaListView {
            id: deviceListView
            width: parent.width
            height: Theme.itemSizeExtraLarge * 2
            clip: true
            model: bleScanModel
            spacing: Theme.paddingLarge
            delegate: ListItem {
                menu: ContextMenu {
                    MenuItem {
                        text: connected ? qsTr("Disconnect") : qsTr("Connect")
                        onClicked: connected ? deviceListView.model.disconnectFromDevice(index) : deviceListView.model.connectToDevice(index)
                    }

                    MenuItem {
                        enabled: connected
                        text: qsTr("Introspect")
                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("AboutDevice.qml"),
                                           {serviceList: deviceListView.model.introspectService(index)})
                        }
                    }
                }

                Column {
                    width: parent.width / 2
                    anchors {
                        left: parent.left
                        leftMargin: Theme.paddingMedium
                    }
                    Label {
                        text: name === "" ? address : name + " - " + address
                    }
                    Label {
                        text: connected ? qsTr("connected") : qsTr("not connected")
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryHighlightColor
                    }
                    Label {
                        text: paired ? qsTr("paired") : qsTr("not paired")
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryHighlightColor
                    }
                }

                Label {
                    text: rssi
                    anchors {
                        right: parent.right
                        rightMargin: Theme.paddingMedium
                        verticalCenter: parent.verticalCenter
                    }
                }
            }

            header: ListItem {
                Row {
                    height: parent.height
                    spacing: Theme.paddingMedium
                    anchors.horizontalCenter: parent.horizontalCenter

                    Label {
                        text: qsTr("scan in process")
                        width: implicitWidth
                        visible: bleScanModel.discovering
                        color: Theme.highlightColor
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    BusyIndicator {
                        running: bleScanModel.discovering
                        visible: running
                        size: BusyIndicatorSize.Small
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
