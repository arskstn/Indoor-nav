// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "blescanmodel.h"
#include <QDebug>
#include <QMap>
#include <QVector>
#include <QFile>
#include <functional>
#include "simpleble/Adapter.h"
#include <functional>
#include <memory>
#include <QDebug>
#include <algorithm>
#include <QtMath>

BleScanModel::BleScanModel(QObject *parent)
    : QAbstractListModel(parent)
{
    try {
        if(SimpleBLE::Adapter::bluetooth_enabled()
                && SimpleBLE::Adapter::get_adapters().size() > 0) {
            init();
        } else {
            m_diagnosticMessage = "Bluetooth is disabled or no adapters found.";
            emit diagnosticMessageChanged();
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error initializing: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
    m_scanInfoTimer.setInterval(2000);
    connect(&m_scanInfoTimer, &QTimer::timeout, this, &BleScanModel::updateScanInfo);
    loadGridData();
}

QVariant BleScanModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_filteredDevices.size()) {
        return QVariant();
    }
    switch (role) {
    case NameRole: return QVariant::fromValue(m_filteredDevices.at(index.row()).get()->identifier());
    case AddressRole: return QVariant::fromValue(m_filteredDevices.at(index.row()).get()->address());
    case AddressTypeRole: return QVariant::fromValue(m_filteredDevices.at(index.row()).get()->bluetoothAddressType());
    case RssiRole: return QVariant::fromValue(m_filteredDevices.at(index.row()).get()->rssi());
    case PairedRole: return QVariant::fromValue(m_filteredDevices.at(index.row()).get()->isPaired());
    case ConnectedRole: return QVariant::fromValue(m_filteredDevices.at(index.row()).get()->isConnetced());
    }
    return QVariant();
}

QHash<int, QByteArray> BleScanModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[AddressRole] = "address";
    roles[AddressTypeRole] = "addressType";
    roles[RssiRole] = "rssi";
    roles[PairedRole] = "paired";
    roles[ConnectedRole] = "connected";
    return roles;
}

int BleScanModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_filteredDevices.size();
}

bool BleScanModel::discovering()
{
    if(m_adapterPower) {
        bool isDiscovering = m_adapter.scan_is_active();
        if (isDiscovering && !m_scanInfoTimer.isActive()) {
            m_scanInfoTimer.start();
        } else if (!isDiscovering && m_scanInfoTimer.isActive()) {
            m_scanInfoTimer.stop();
        }
        return isDiscovering;
    }
    return false;
}

void BleScanModel::setDiscovering(bool discovering)
{
    try {
        if(discovering) {
            m_adapter.scan_start();
        } else {
            m_adapter.scan_stop();
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error during discovery: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
    emit discoveringChanged();
}

void BleScanModel::startContinuousScan()
{
    try {
        if (!m_adapterPower) {
            m_diagnosticMessage = "Error: Bluetooth adapter is not powered.";
            emit diagnosticMessageChanged();
            return;
        }
        if (!m_adapter.scan_is_active()) {
            m_adapter.scan_start();
            emit discoveringChanged();
        }
        m_scanInfoTimer.start();
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error starting continuous scan: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
}

void BleScanModel::stopContinuousScan()
{
    try {
        if (m_adapter.scan_is_active()) {
            m_adapter.scan_stop();
            m_scanInfoTimer.stop();
            emit discoveringChanged();
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error stopping continuous scan: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
}

void BleScanModel::connectToDevice(int index)
{
    if(index >= m_filteredDevices.size()) {
        m_diagnosticMessage = QString("Error: Invalid device index %1").arg(index);
        emit diagnosticMessageChanged();
        qDebug()<< Q_FUNC_INFO << index;
        return;
    }
    try {
        m_filteredDevices.at(index)->connectToDevice();
    } catch (SimpleBLE::Exception::OperationFailed &ex) {
        m_diagnosticMessage = QString("Error connecting to device: %1").arg(ex.what());
        emit diagnosticMessageChanged();
        qDebug() << ex.what();
    }
}

void BleScanModel::disconnectFromDevice(int index)
{
    if(index >= m_filteredDevices.size()) {
        m_diagnosticMessage = QString("Error: Invalid device index %1").arg(index);
        emit diagnosticMessageChanged();
        return;
    }
    try {
        m_filteredDevices.at(index)->disconnectFromDevice();
    } catch (SimpleBLE::Exception::OperationFailed &ex) {
        m_diagnosticMessage = QString("Error disconnecting from device: %1").arg(ex.what());
        emit diagnosticMessageChanged();
        qDebug() << ex.what();
    }
}

QList<QObject*> BleScanModel::introspectService(int index)
{
    QList<QObject*> tempServicesList{};
    if(index >= m_filteredDevices.size()) {
        m_diagnosticMessage = QString("Error: Invalid device index %1 for introspection").arg(index);
        emit diagnosticMessageChanged();
        return tempServicesList;
    }
    try {
        if(m_filteredDevices.at(index)->isConnetced()) {
            return m_filteredDevices.at(index)->services();
        } else {
            m_diagnosticMessage = QString("Error: Device at index %1 is not connected").arg(index);
            emit diagnosticMessageChanged();
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error introspecting services: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
    return tempServicesList;
}

void BleScanModel::clear()
{
    beginRemoveRows(QModelIndex{},0,m_filteredDevices.size());
    m_filteredDevices.clear();
    m_listDevices.clear();
    endRemoveRows();
}

void BleScanModel::clearScanInfo()
{
    m_scanInfo.clear();
    emit scanInfoChanged();
}

void BleScanModel::clearFilteredDevices()
{
    beginResetModel();
    m_filteredDevices.clear();
    endResetModel();
}

void BleScanModel::setPowered(bool value)
{
    m_adapterPower = value;
    if(value) {
        init();
    } else {
        m_diagnosticMessage = "Bluetooth adapter powered off.";
        emit diagnosticMessageChanged();
        clear();
    }
}

void BleScanModel::updateDeviceInfo(SimpleBLE::Peripheral peripheral)
{
    QString identifier = QString::fromStdString(peripheral.identifier());
    int rssi = peripheral.rssi();

    QStringList allowedIdentifiers = {"ESP32_PICO_V3_BLE_1", "ESP32_PICO_V3_BLE_2",
                                     "ESP32_PICO_V3_BLE_3", "ESP32_PICO_V3_BLE_4"};
    if (!allowedIdentifiers.contains(identifier)) {
        return;
    }

    if (m_rssiHistory.contains(identifier)) {
        QList<int> &history = m_rssiHistory[identifier];
        history.append(rssi);
        if (history.size() > 5) {
            history.removeFirst();
        }
    } else {
        m_rssiHistory[identifier] = QList<int>() << rssi;
    }
    qDebug() << "Updated RSSI for" << identifier << ": " << rssi << "History:" << m_rssiHistory[identifier];

    for (int index = 0; index < m_listDevices.size(); ++index) {
        if (m_listDevices.at(index)->address() == QString::fromStdString(peripheral.address())) {
            m_listDevices.at(index)->updatePeripheral(peripheral);
            updateFilteredDevices();
            calculateTriangulation();
            return;
        }
    }
}
void BleScanModel::searchDevice(SimpleBLE::Peripheral peripheral)
{
    auto it = std::find_if(m_listDevices.cbegin(), m_listDevices.cend(),
                           [&](std::shared_ptr<PeripheralAdapter> arg){
                               return arg->address().contains(QString::fromStdString(peripheral.address()));
                           });

    if(it == m_listDevices.cend()) {
        auto newDevice = std::make_shared<PeripheralAdapter>(peripheral);
        m_listDevices.append(newDevice);
        updateFilteredDevices();
    }
}

void BleScanModel::setSortByNameEnabled(bool enabled)
{
    if (m_sortByNameEnabled != enabled) {
        m_sortByNameEnabled = enabled;
        updateFilteredDevices();
        emit sortByNameEnabledChanged();
    }
}

void BleScanModel::sortByName(bool enabled)
{
    setSortByNameEnabled(enabled);
}

void BleScanModel::updateScanInfo()
{
    try {
        if (m_adapter.scan_is_active()) {
            m_adapter.scan_stop();
            m_adapter.scan_start();
        }
        QStringList deviceInfo;
        deviceInfo << QString("Total devices found: %1").arg(m_listDevices.size());
        for (const auto &device : m_listDevices) {
            if (device) {
                QString info = QString("Name: %1, Address: %2, RSSI: %3, Address Type: %4, Paired: %5, Connected: %6")
                               .arg(device->identifier())
                               .arg(device->address())
                               .arg(device->rssi())
                               .arg(device->bluetoothAddressType())
                               .arg(device->isPaired() ? "Yes" : "No")
                               .arg(device->isConnetced() ? "Yes" : "No");
                deviceInfo << info;
            }
        }
        m_scanInfo = deviceInfo.join("\n");
        emit scanInfoChanged();
        updateFilteredDevices();
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error updating scan info: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
}

void BleScanModel::updateFilteredDevices()
{
    beginResetModel();
    m_filteredDevices.clear();
    QStringList allowedNames = {
        "ESP32_PICO_V3_BLE_1",
        "ESP32_PICO_V3_BLE_2",
        "ESP32_PICO_V3_BLE_3",
        "ESP32_PICO_V3_BLE_4"
    };
    for (const auto &device : m_listDevices) {
        if (allowedNames.contains(device->identifier()) && device->rssi() != 0) {
            m_filteredDevices.append(device);
        }
    }
    if (m_sortByNameEnabled) {
        std::sort(m_filteredDevices.begin(), m_filteredDevices.end(),
                  [](const std::shared_ptr<PeripheralAdapter> &a, const std::shared_ptr<PeripheralAdapter> &b) {
                      return a->identifier() < b->identifier();
                  });
    }
    endResetModel();
}

void BleScanModel::loadGridData()
{
    QFile file(":/icons/kartasmall_grid.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_diagnosticMessage = QString("Failed to open kartasmall_grid.csv: %1").arg(file.errorString());
        emit diagnosticMessageChanged();
        return;
    }

    m_gridData.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        m_gridData.append(line);
    }
    file.close();

    if (m_gridData.size() != 92) {
        m_diagnosticMessage = QString("Invalid grid data: expected 92 rows, got %1").arg(m_gridData.size());
        emit diagnosticMessageChanged();
        return;
    }
    emit gridDataChanged();
    m_diagnosticMessage = "Grid data loaded successfully.";
    emit diagnosticMessageChanged();
}

void BleScanModel::init()
{
    try {
        m_adapter = SimpleBLE::Adapter::get_adapters().at(0);
        m_adapter.set_callback_on_scan_found(
                    std::bind(&BleScanModel::searchDevice, this, std::placeholders::_1)
                    );
        m_adapter.set_callback_on_scan_updated(
                    std::bind(&BleScanModel::updateDeviceInfo, this, std::placeholders::_1)
                    );
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error initializing adapter: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
}

QPointF BleScanModel::position() const
{
    return m_position;
}

void BleScanModel::updatePosition(const QPointF &pos)
{
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged();
    }
}
void BleScanModel::calculateTriangulation()
{
    QPointF pos = triangulatePosition();
    if (pos.x() >= 0 && pos.y() >= 0) {
        updatePosition(pos);
    } else {
        qDebug() << "Invalid triangulation result, position not updated";
    }
}

void BleScanModel::calculateNearestNeighbor()
{
    QPointF pos = nearestNeighborPosition();
    updatePosition(pos);
}

double BleScanModel::calculateDistance(int rssi) const
{
    const double txPower = -50.0;
    const double n = 4.0;
    if (rssi == 0) return 0.0;
    return qPow(10.0, (txPower - rssi) / (10.0 * n));
}

QPointF BleScanModel::triangulatePosition() const
{

    const QMap<QString, QPointF> beaconPositions = {
        {"ESP32_PICO_V3_BLE_1", {1.3, 9.0}},  // Z1
        {"ESP32_PICO_V3_BLE_2", {11.2, 6.9}}, // Z2
        {"ESP32_PICO_V3_BLE_3", {11.3, 2.7}}, // Z3
        {"ESP32_PICO_V3_BLE_4", {5.6, 1.6}}   // Z4
    };

    QVector<QPointF> beacons;
    QVector<double> distances;
    for (const QString &name : beaconPositions.keys()) {
        if (m_rssiHistory.contains(name) && !m_rssiHistory[name].isEmpty()) {
            int avgRssi = 0;
            const QList<int> &history = m_rssiHistory[name];
            int sum = 0;
            for (int val : history) sum += val;
            avgRssi = history.isEmpty() ? 0 : sum / history.size();
            double distance = calculateDistance(avgRssi);
            beacons << beaconPositions[name];
            distances << distance;
            qDebug() << "Distance for" << name << ": " << distance << "m (RSSI avg:" << avgRssi << ")";
        } else {
            qDebug() << "No data for" << name;
        }
    }

    if (distances.size() >= 3) {
        QVector<double> validDistances;
        QVector<QPointF> validBeacons;
        for (int i = 0; i < 3 && i < distances.size(); ++i) {
            if (distances[i] > 0) {
                validDistances << distances[i];
                validBeacons << beacons[i];
            }
        }
        if (validDistances.size() >= 3) {
            double x = (qPow(validDistances[0], 2) - qPow(validDistances[1], 2) + qPow(validBeacons[1].x() - validBeacons[0].x(), 2) +
                        qPow(validBeacons[1].y() - validBeacons[0].y(), 2)) / (2 * (validBeacons[1].x() - validBeacons[0].x()));
            double y = (qPow(validDistances[0], 2) - qPow(validDistances[2], 2) + qPow(validBeacons[2].x() - validBeacons[0].x(), 2) +
                        qPow(validBeacons[2].y() - validBeacons[0].y(), 2)) / (2 * (validBeacons[2].x() - validBeacons[0].x())) +
                       (x - validBeacons[0].x()) * (validBeacons[2].y() - validBeacons[0].y()) / (validBeacons[2].x() - validBeacons[0].x());
            x = qBound(0.0, x, 11.4);
            y = qBound(0.0, y, 9.2);
            QPointF pos(x, y);
            qDebug() << "Triangulated position: (" << pos.x() << ", " << pos.y() << ") m";
            return pos;
        }
    }
    qDebug() << "Triangulation failed: need at least 3 valid beacons, found" << distances.size();
    return QPointF(-1, -1);
}

QPoint BleScanModel::positionInPixels() const
{
    qDebug() << "Calculating positionInPixels - m_position:" << m_position.x() << "," << m_position.y();
    const qreal mapWidthMeters = 11.4;  // 114 клетки по 0.1 м
    const qreal mapHeightMeters = 9.2;  // 92 клетки по 0.1 м
    const int mapWidthPixels = 114;     // Ширина карты в пикселях
    const int mapHeightPixels = 92;     // Высота карты в пикселях

    qreal xPercent = m_position.x() / mapWidthMeters;
    qreal yPercent = m_position.y() / mapHeightMeters;
    int xPixel = qRound(xPercent * mapWidthPixels);
    int yPixel = mapHeightPixels - 1 - qRound(yPercent * mapHeightPixels);

    xPixel = qBound(0, xPixel, mapWidthPixels - 1);
    yPixel = qBound(0, yPixel, mapHeightPixels - 1);

    qDebug() << "Position in pixels:" << xPixel << "," << yPixel;
    return QPoint(xPixel, yPixel);
}

void BleScanModel::updateDiagnosticMessage()
{
    QString msg = "RSSI and History:\n";
    for (auto it = m_rssiHistory.constBegin(); it != m_rssiHistory.constEnd(); ++it) {
        QString historyStr;
        for (int val : it.value()) {
            if (!historyStr.isEmpty()) historyStr += ", ";
            historyStr += QString::number(val);
        }
        int avgRssi = 0;
        if (!it.value().isEmpty()) {
            int sum = 0;
            for (int val : it.value()) {
                sum += val;
            }
            avgRssi = sum / it.value().size();
        }
        msg += QString("%1: RSSI avg=%2, History=%3\n")
               .arg(it.key())
               .arg(avgRssi)
               .arg(historyStr);
    }
    if (m_position.x() >= 0 && m_position.y() >= 0) {
        msg += QString("Position: (%1, %2) m\n").arg(m_position.x()).arg(m_position.y());
    } else {
        msg += "Position: Not calculated\n";
    }
    qDebug() << "Diagnostic:" << msg;
    m_diagnosticMessage = msg;
    emit diagnosticMessageChanged();
}

QPointF BleScanModel::nearestNeighborPosition() const
{
    QString bestBeacon;
    int maxRssi = -1000;
    for (auto it = m_rssiHistory.constBegin(); it != m_rssiHistory.constEnd(); ++it) {
        if (!it.value().isEmpty()) {
            int avgRssi = 0;
            int sum = 0;
            for (int rssi : it.value()) sum += rssi;
            avgRssi = it.value().size() > 0 ? sum / it.value().size() : 0;
            if (avgRssi > maxRssi) {
                maxRssi = avgRssi;
                bestBeacon = it.key();
            }
        }
    }

    const QMap<QString, QPointF> beaconPositions = {
        {"ESP32_PICO_V3_BLE_1", {1.3, 9.0}},  // Z1
        {"ESP32_PICO_V3_BLE_2", {11.2, 6.9}}, // Z2
        {"ESP32_PICO_V3_BLE_3", {11.3, 2.7}}, // Z3
        {"ESP32_PICO_V3_BLE_4", {5.6, 1.6}}   // Z4
    };

    if (beaconPositions.contains(bestBeacon)) {
        qDebug() << "Nearest neighbor position:" << bestBeacon << "at" << beaconPositions[bestBeacon];
        return beaconPositions[bestBeacon];
    }
    qDebug() << "Nearest neighbor not found";
    return QPointF(-1, -1); // Ошибка
}
