// SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "scanmodel.h"
#include <QDebug>
#include <QMap>
#include <QVector>
#include <functional>
#include <simplebluez/Adapter.h>
#include <simpledbus/base/Message.h>
#include <simpledbus/advanced/Interface.h>
#include <simplebluez/Bluez.h>
#include <functional>

ScanModel::ScanModel(QObject *parent) : QAbstractListModel(parent)
{
    m_bluez.init();
    if(!m_bluez.get_adapters().empty()) {
        m_adapter = m_bluez.get_adapters().at(0);
        qDebug()<< Q_FUNC_INFO << QString::fromStdString(m_adapter->identifier()) << QString::fromStdString(m_adapter->address());
        m_adapter->set_on_device_updated(
                    std::bind(&ScanModel::updateDeviceInfo, this, std::placeholders::_1)
                    );
    }
    m_timer.setInterval(100);
    connect(&m_timer, &QTimer::timeout, this, &ScanModel::asyncRead);
}

int ScanModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_listDevices.isEmpty() ? 0 : m_listDevices.size();
}

QVariant ScanModel::data(const QModelIndex &index, int role) const
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
    case DeviceNameRole: return QVariant::fromValue(QString(""));
    }
    return QVariant();
}

QHash<int, QByteArray> ScanModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        {NameRole, "name"},
        {AddressRole, "address"},
        {AddressTypeRole, "addressType"},
        {RssiRole, "rssi"},
        {PairedRole, "paired"},
        {ConnectedRole, "connected"},
        {DeviceNameRole, "deviceName"},
    };
    return roles;
}

bool ScanModel::discovering() const
{
    if(m_adapter.get() == nullptr)
        return false;
    return m_adapter->discovering();
}

bool ScanModel::powered() const
{
    if(m_adapter.get() == nullptr)
        return false;
    return m_adapter->powered();
}

void ScanModel::setDiscovering(bool discovering)
{
    if(m_adapter.get() == nullptr)
        return;
    if(discovering) {
        m_adapter->discovery_start();
        m_timer.start();
    }else{
        m_adapter->discovery_stop();
        m_timer.stop();
    }
    emit discoveringChanged();
}

void ScanModel::connectToDevice(int index)
{
    if(index > m_listDevices.size()) {
        qDebug()<< Q_FUNC_INFO << index;
        return;
    }

    QMap map(m_listDevices.at(index)->interfaces());
    for(auto it: map.keys()) {
        if(QString::fromStdString(it).contains("Device1") ) {
            auto msg = map.value(it)->create_method_call("Connect");
            SimpleDBus::Connection conn(static_cast<DBusBusType>(BusType::System));
            conn.init();
            conn.send(msg);
        }
    }
}

void ScanModel::disconnectFromDevice(int index)
{
    if(index > m_listDevices.size()) {
        return;
    }
    if(m_listDevices.at(index)->valid() && m_listDevices.at(index) != nullptr) {
        m_listDevices.at(index)->disconnect();
    }
}

QList<QObject*>  ScanModel::introspectService(int index)
{
    QList<QObject*> tempServicesList{};
    if(index > m_listDevices.size()) {
        return tempServicesList;
    }
    if(m_listDevices.at(index)->valid() && m_listDevices.at(index) != nullptr) {
        for(auto service: m_listDevices.at(index)->services()) {
            tempServicesList.append(new ServiceImp(service));
        }
    }
    return tempServicesList;
}

void ScanModel::clear()
{
    if(discovering()) {
        setDiscovering(false);
    }
    beginRemoveRows(QModelIndex{}, 0, m_listDevices.size() - 1);
    m_listDevices.clear();
    endRemoveRows();
}

void ScanModel::asyncRead()
{
    m_bluez.run_async();
}

void ScanModel::updateDeviceInfo(std::shared_ptr<SimpleBluez::Device> device)
{
    if(!m_listDevices.contains(device)) {
        beginInsertRows(QModelIndex(), m_listDevices.size(), m_listDevices.size());
        m_listDevices.append(device);
        endInsertRows();
    }else{
        auto indexItem = createIndex(m_listDevices.indexOf(device), 0);
        emit dataChanged(indexItem, indexItem);
    }
    for(auto &it: m_listDevices) {
        if(it.get() == nullptr) {
            auto indexItem = m_listDevices.indexOf(it);
            beginRemoveRows(QModelIndex(), indexItem, indexItem);
            endRemoveRows();
        }
    }
}
