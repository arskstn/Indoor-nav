import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.Controls 2.5

ApplicationWindow {
    id: window

    property int button1Count: 0
    property int button2Count: 0

    initialPage: Page {
        Column {
            anchors.centerIn: parent
            spacing: Theme.paddingLarge

            Button {
                text: "Button 1 (Clicks: " + button1Count + ")"
                onClicked: {
                    button1Count++;
                }
            }

            Button {
                text: "Button 2 (Clicks: " + button2Count + ")"
                onClicked: {
                    button2Count++;
                }
            }

            Label {
                text: "Button 1 Count: " + button1Count
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                text: "Button 2 Count: " + button2Count
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
            }

            Label {
                text: "BLE Scan Results:"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
            }

            TextArea {
                id: consoleArea
                width: parent.width
                height: 200
                readOnly: true
                text: bleScanner.scanResults.stringList.join("\n")
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                background: Rectangle {
                    color: Theme.secondaryHighlightColor
                }
            }

            Button {
                text: "Start BLE Scan"
                onClicked: bleScanner.startScanning()
            }

            Button {
                text: "Stop BLE Scan"
                onClicked: bleScanner.stopScanning()
            }
        }

        Connections {
            target: bleScanner
            function onScanResultsChanged() {
                consoleArea.text = bleScanner.scanResults.stringList.join("\n")
            }
        }
    }

    Component.onCompleted: {
        rootContext()->setContextProperty("bleScanner", new BLEScanner(this));
    }
}
