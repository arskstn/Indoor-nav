import QtQuick 2.0
import Sailfish.Silica 1.0
import com.example 1.0

Page {
    id: page
    SilicaFlickable {
        anchors.fill: parent
        Column {
            width: page.width
            PageHeader { title: "BLE Beacons" }
            Label { id: statusLabel; text: "Нажми для сканирования" }
            Button {
                text: "Сканировать"
                onClicked: {
                    statusLabel.text = "Сканирование...";
                    bleScanner.startScan();
                }
            }
        }
    }

    BleScanner { id: bleScanner }

    Component.onCompleted: {
        bleScanner.beaconDataReady.connect(function(data) {
            statusLabel.text = data;
        });
    }
}
