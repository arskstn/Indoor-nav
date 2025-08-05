// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "bluezscanmodel.h"
#include <QDebug>

BluezScanModel::BluezScanModel(QObject *parent)
    : QAbstractListModel(parent)
{
    init();
    m_scanInfoTimer.setInterval(2000);
    connect(&m_scanInfoTimer, &QTimer::timeout, this, &BluezScanModel::updateScanInfo);
}

QVariant BluezScanModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > m_listDevices.size()) {
        qDebug()<< Q_FUNC_INFO << index.row() << m_listDevices.size();
        return QVariant();
    }

    switch (role) {
    case NameRole: return QVariant::fromValue(QString::fromStdString(m_listDevices.at(index.row())->name()));
    case AddressRole: return QVariant::fromValue(QString::fromStdString(m_listDevices.at(index.row())->address()));
    case AddressTypeRole: return QVariant::fromValue(QString::fromStdString(m_listDevices.at(index.row())->address_type()));
    case RssiRole: return QVariant::fromValue(qint16(m_listDevices.at(index.row())->rssi()));
    case PairedRole: return QVariant::fromValue(m_listDevices.at(index.row())->paired());
    case ConnectedRole: return QVariant::fromValue(m_listDevices.at(index.row())->connected());
    }
    return QVariant();
}

QHash<int, QByteArray> BluezScanModel::roleNames() const
{
    return this->BaseModel::roleNames();
}

int BluezScanModel::rowCount(const QModelIndex &parent) const
{
    return this->BaseModel::rowCount(parent);
}

bool BluezScanModel::discovering()
{
    if(m_adapter.get() == nullptr)
        return false;
    bool isDiscovering = m_adapter->discovering();
    if (isDiscovering && !m_scanInfoTimer.isActive()) {
        m_scanInfoTimer.start();
    } else if (!isDiscovering && m_scanInfoTimer.isActive()) {
        m_scanInfoTimer.stop();
    }
    return isDiscovering;
}

void BluezScanModel::setDiscovering(bool discovering)
{
    if(m_adapter.get() == nullptr){
        m_diagnosticMessage = "Error: Bluetooth adapter is not available.";
        emit diagnosticMessageChanged();
        emit discoveringChanged();
        return;
    }
    try {
        if(discovering) {
            qDebug() << "Start discovery!";
            m_adapter->discovery_start();
            m_timer.start();
        } else {
            qDebug() << "Stop discovery!";
            m_adapter->discovery_stop();
            m_timer.stop();
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error during discovery: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
    emit discoveringChanged();
}

void BluezScanModel::connectToDevice(int index)
{
    if(index > m_listDevices.size()) {
        m_diagnosticMessage = QString("Error: Invalid device index %1").arg(index);
        emit diagnosticMessageChanged();
        qDebug()<< Q_FUNC_INFO << index;
        return;
    }

    try {
        const QMap map(m_listDevices.at(index)->interfaces());
        for(const auto &it: map.keys()) {
            if(QString::fromStdString(it).contains("Device1") ) {
                auto msg = map.value(it)->create_method_call("Connect");
                SimpleDBus::Connection conn(static_cast<DBusBusType>(BusType::System));
                conn.init();
                conn.send(msg);
            }
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error connecting to device: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
}

void BluezScanModel::disconnectFromDevice(int index)
{
    if(index > m_listDevices.size()) {
        m_diagnosticMessage = QString("Error: Invalid device index %1").arg(index);
        emit diagnosticMessageChanged();
        return;
    }
    try {
        if(m_listDevices.at(index)->valid() && m_listDevices.at(index) != nullptr) {
            m_listDevices.at(index)->disconnect();
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error disconnecting from device: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
}

QList<QObject *> BluezScanModel::introspectService(int index)
{
    QList<QObject*> tempServicesList{};
    if(index > m_listDevices.size()) {
        m_diagnosticMessage = QString("Error: Invalid device index %1 for introspection").arg(index);
        emit diagnosticMessageChanged();
        return tempServicesList;
    }
    try {
        if(m_listDevices.at(index)->valid() && m_listDevices.at(index) != nullptr) {
            for(const auto &service: m_listDevices.at(index)->services()) {
                tempServicesList.append(new ServiceImp(service));
            }
        }
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error introspecting services: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
    return tempServicesList;
}

void BluezScanModel::clear()
{
    beginRemoveRows(QModelIndex{},0,m_listDevices.size());
    this->BaseModel::clear();
    endRemoveRows();
}

void BluezScanModel::setPowered(bool value)
{
    m_adapterPower = value;
    if(value) {
        init();
    } else {
        m_diagnosticMessage = "Bluetooth adapter powered off.";
        emit diagnosticMessageChanged();
        m_adapter.reset();
        clear();
    }
}

void BluezScanModel::asyncRead()
{
    try {
        m_bluez.run_async();
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error in async read: %1").arg(e.what());
        emit diagnosticMessageChanged();
    }
}

void BluezScanModel::updateDeviceInfo(std::shared_ptr<SimpleBluez::Device> device)
{
    qDebug() << "Find device " << QString::fromStdString(device.get()->address());
    if(!m_listDevices.contains(device)) {
        beginInsertRows(QModelIndex(), m_listDevices.size(), m_listDevices.size());
        m_listDevices.append(device);
        endInsertRows();
    } else {
        auto indexItem = createIndex(m_listDevices.indexOf(device), 0);
        emit dataChanged(indexItem, indexItem);
    }
    for(auto &it: m_listDevices) {
        if(it.get() == nullptr) {
            const auto indexItem = m_listDevices.indexOf(it);
            beginRemoveRows(QModelIndex(), indexItem, indexItem);
            endRemoveRows();
        }
    }
}

void BluezScanModel::updateScanInfo()
{
    QStringList deviceInfo;
    deviceInfo << QString("Total devices found: %1").arg(m_listDevices.size());
    for (const auto &device : m_listDevices) {
        if (device && device->valid()) {
            QString info = QString("Address: %1, Name: %2, RSSI: %3")
                           .arg(QString::fromStdString(device->address()))
                           .arg(QString::fromStdString(device->name()))
                           .arg(device->rssi());
            deviceInfo << info;
        }
    }
    m_scanInfo = deviceInfo.join("\n");
    emit scanInfoChanged();
}

void BluezScanModel::init()
{
    try {
        m_bluez.init();
        if(!m_bluez.get_adapters().empty()) {
            m_filter.Transport = SimpleBluez::Adapter::DiscoveryFilter::TransportType::LE;
            m_adapter = m_bluez.get_adapters().at(0);
            m_adapter->discovery_filter(m_filter);

            qDebug()<< Q_FUNC_INFO << QString::fromStdString(m_adapter->identifier()) << QString::fromStdString(m_adapter->address());
            m_adapter->set_on_device_updated(
                        std::bind(&BluezScanModel::updateDeviceInfo, this, std::placeholders::_1)
                        );
        } else {
            m_diagnosticMessage = "No Bluetooth adapters found.";
            emit diagnosticMessageChanged();
        }
        m_timer.setInterval(10);
        connect(&m_timer, &QTimer::timeout, this, &BluezScanModel::asyncRead);
    } catch (const std::exception &e) {
        m_diagnosticMessage = QString("Error initializing adapter: %1").arg(e.what());
        emit diagnosticMessageChanged();
        m_adapter.reset();
    }
}

QStringList ServiceImp::introspectCharacterisitics(){
    QStringList listChar;
    try {
        for (auto characteristic : m_service->characteristics()) {
            listChar.append(QString::fromStdString(characteristic->uuid()));
        }
    } catch (const std::exception &e) {
        qDebug() << "Error introspecting characteristics:" << e.what();
    }
    return listChar;
}

QString ServiceImp::uuid(){
    QString result;
    try {
        result = QString::fromStdString(m_service->uuid());
    } catch (SimpleDBus::Exception::InterfaceNotFoundException& e) {
        result = QString::fromStdString(e.what());
    }
    return result;
}
