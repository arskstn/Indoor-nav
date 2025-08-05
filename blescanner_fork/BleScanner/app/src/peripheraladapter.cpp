// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "peripheraladapter.h"

PeripheralAdapter::PeripheralAdapter(const SimpleBLE::Peripheral &peripheral
                                   , QObject *parent)
    : QObject(parent)
{
    updateProperties(peripheral);
}

void PeripheralAdapter::updatePeripheral(const SimpleBLE::Peripheral &peripheral)
{
    updateProperties(peripheral);
}

const QString &PeripheralAdapter::identifier() const
{
    return m_identifier;
}

void PeripheralAdapter::setIdentifier(const QString &newIdentifier)
{
    m_identifier = newIdentifier;
}

const QString &PeripheralAdapter::address() const
{
    return m_address;
}

void PeripheralAdapter::setAddress(const QString &newAddress)
{
    m_address = newAddress;
}

int PeripheralAdapter::bluetoothAddressType() const
{
    return m_bluetoothAddressType;
}

void PeripheralAdapter::setBluetoothAddressType(int newBluetoothAddressType)
{
    m_bluetoothAddressType = newBluetoothAddressType;
}

int PeripheralAdapter::rssi() const
{
    return m_rssi;
}

void PeripheralAdapter::setRssi(int newRssi)
{
    m_rssi = newRssi;
}

bool PeripheralAdapter::isPaired() const
{
    return m_isPaired;
}

void PeripheralAdapter::setIsPaired(bool newIs_paired)
{
    m_isPaired = newIs_paired;
}

bool PeripheralAdapter::isConnetced() const
{
    return m_isConnetced;
}

void PeripheralAdapter::setIsConnetced(bool newIsConnetced)
{
    m_isConnetced = newIsConnetced;
}

void PeripheralAdapter::connectToDevice()
{
    m_peripheral.connect();
}

void PeripheralAdapter::disconnectFromDevice()
{
    m_peripheral.disconnect();
}

QList<QObject *> PeripheralAdapter::services()
{
    QList<QObject*> servicesList;
    for(const auto &it: m_peripheral.services()) {
        servicesList.append(new ServiceBLEAdapter(it));
    }
    return servicesList;
}

void PeripheralAdapter::updateProperties(const SimpleBLE::Peripheral &peripheral)
{
    m_peripheral = peripheral;
    m_address = QString::fromStdString(m_peripheral.address());
    m_identifier = QString::fromStdString(m_peripheral.identifier());
    m_bluetoothAddressType = m_peripheral.address_type();
    m_rssi = m_peripheral.rssi();
    m_isPaired = m_peripheral.is_paired();
    m_isConnetced = m_peripheral.is_connected();
}

QStringList ServiceBLEAdapter::introspectCharacterisitics(){
    QStringList result;
    for(auto &characteristic: m_service.characteristics()){
        result.append(QString::fromStdString(characteristic.uuid()));
    }
    return result;
}
