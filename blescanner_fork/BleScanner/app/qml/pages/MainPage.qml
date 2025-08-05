// SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.omp.SimpleBLE 1.0

Page {
    objectName: "mainPage"
    allowedOrientations: Orientation.All

    PageHeader {
        id: pageHeader
        objectName: "pageHeader"
        title: qsTr("BLE Scanner")
        extraContent.children: [
            IconButton {
                objectName: "aboutButton"
                icon.source: "image://theme/icon-m-about"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        ]
    }

    BleScanModel {
        id: bleScanModel
        powered: adapterStatus.powered
    }

    Column {
        id: switchGroup
        anchors {
            top: pageHeader.bottom
            left: parent.left
            right: parent.right
        }
        spacing: Theme.paddingMedium

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: Theme.paddingMedium
            Row {
                spacing: Theme.paddingMedium
                Button {
                    enabled: adapterStatus.powered
                    visible: enabled
                    text: qsTr("Start")
                    onClicked: {
                        bleScanModel.startContinuousScan()
                    }
                }
                Button {
                    enabled: bleScanModel.discovering
                    visible: enabled
                    text: qsTr("Stop")
                    onClicked: {
                        bleScanModel.stopContinuousScan()
                    }
                }
                Button {
                    text: qsTr("Clear Scan")
                    onClicked: {
                        bleScanModel.clearScanInfo()
                    }
                }
            }
            Row {
                spacing: Theme.paddingMedium
                Button {
                    text: qsTr("Clear List")
                    onClicked: {
                        bleScanModel.clearFilteredDevices()
                    }
                }
                Button {
                    text: qsTr("RSSI Fingerprinting")
                    onClicked: pageStack.push(Qt.resolvedUrl("RSSIFingerprinting.qml"))
                }
                Button {
                    text: qsTr("IMU Fusion")
                    onClicked: pageStack.push(Qt.resolvedUrl("IMUFusion.qml"))
                }
                Button {
                            text: qsTr("Map View")
                            onClicked: pageStack.push(Qt.resolvedUrl("MapPage.qml"), {bleScanModel: bleScanModel})
                        }
            }
        }

        Button {
            text: qsTr("Update Position")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                bleScanModel.calculateTriangulation()
                bleScanModel.updateDiagnosticMessage()
            }
        }

        TextSwitch {
            id: sortSwitch
            text: qsTr("Sort by Name")
            description: qsTr("Sort discovered devices alphabetically by name")
            checked: bleScanModel.sortByNameEnabled
            onCheckedChanged: {
                bleScanModel.setSortByNameEnabled(checked)
            }
        }
        TextArea {
            id: diagnosticArea
            width: parent.width
            height: Theme.itemSizeMedium
            readOnly: true
            text: bleScanModel.diagnosticMessage
            label: qsTr("Diagnostic Messages")
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        TextArea {
            id: scanInfoArea
            width: parent.width
            height: Theme.itemSizeExtraLarge * 2
            readOnly: true
            text: bleScanModel.scanInfo
            label: qsTr("Scan Information")
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
        }
    }

    SilicaListView {
        id: deviceListView
        anchors {
            top: switchGroup.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
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

    Label {
        id: poweredLabel
        anchors.centerIn: parent
        visible: !bleScanModel.discovering && deviceListView.count == 0
        text: adapterStatus.powered ? qsTr("Powered On") : qsTr("Powered Off")
    }
}
