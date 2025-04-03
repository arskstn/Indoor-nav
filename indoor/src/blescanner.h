#ifndef BLESCANNER_H
#define BLESCANNER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class BleScanner : public QObject {
    Q_OBJECT
public:
    explicit BleScanner(QObject *parent = nullptr);
    void startScan();

signals:
    void beaconDataReady(const QString &data);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void scanFinished();
    void onReplyFinished(QNetworkReply *reply);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QNetworkAccessManager *networkManager;
    QMap<QString, int> beacons; // MAC -> ID
    QMap<QString, int> rssiValues; // MAC -> RSSI
};

#endif
