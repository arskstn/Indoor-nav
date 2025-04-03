#include "blescanner.h"

BleScanner::BleScanner(QObject *parent) : QObject(parent) {
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    networkManager = new QNetworkAccessManager(this);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BleScanner::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BleScanner::scanFinished);
    connect(networkManager, &QNetworkAccessManager::finished, this, &BleScanner::onReplyFinished);

    // Задаём известные маяки (MAC -> ID)
    beacons["E8:6B:EA:2B:CE:78"] = 1; // Левый нижний
    beacons["E8:6B:EA:2B:02:30"] = 2; // Правый нижний
    beacons["E8:6B:EA:2B:02:08"] = 3; // Левый верхний
}

void BleScanner::startScan() {
    rssiValues.clear();
    discoveryAgent->start();
}

void BleScanner::deviceDiscovered(const QBluetoothDeviceInfo &info) {
    QString mac = info.address().toString().toUpper();
    if (beacons.contains(mac)) {
        rssiValues[mac] = info.rssi();
    }
}

void BleScanner::scanFinished() {
    QString data;
    for (auto it = rssiValues.constBegin(); it != rssiValues.constEnd(); ++it) {
        QString mac = it.key();
        int id = beacons[mac];
        int rssi = it.value();
        data += QString("Клиент: маяк id=%1, ble mac addr=%2, ble rssi=%3;").arg(id).arg(mac).arg(rssi);
    }
    if (!data.isEmpty()) {
        QNetworkRequest request(QUrl("http://192.168.50.1:5000"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
        networkManager->post(request, data.toUtf8());
        emit beaconDataReady(data);
    }
}

void BleScanner::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        emit beaconDataReady("Ошибка отправки: " + reply->errorString());
    }
    reply->deleteLater();
}
