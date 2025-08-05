// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef PERIPHERALADAPTER_H
#define PERIPHERALADAPTER_H

#include <QObject>
#include <simpleble/SimpleBLE.h>
#include "simpleble/Characteristic.h"

class PeripheralAdapter : public QObject
{
    Q_OBJECT
public:
    explicit PeripheralAdapter(const SimpleBLE::Peripheral &peripheral,
                               QObject *parent = nullptr);

    virtual ~PeripheralAdapter() = default;

    void updatePeripheral(const SimpleBLE::Peripheral &peripheral);

    const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    const QString &address() const;
    void setAddress(const QString &newAddress);

    int bluetoothAddressType() const;
    void setBluetoothAddressType(int newBluetoothAddressType);

    int rssi() const;
    void setRssi(int newRssi);

    bool isPaired() const;
    void setIsPaired(bool newIs_paired);

    bool isConnetced() const;
    void setIsConnetced(bool newIsConnetced);

    void connectToDevice();
    void disconnectFromDevice();

    QList<QObject*> services();

private:
    void updateProperties(const SimpleBLE::Peripheral &peripheral);

    QString m_identifier;
    QString m_address;
    int m_bluetoothAddressType;
    int m_rssi;
    bool m_isPaired;
    bool m_isConnetced;
    SimpleBLE::Peripheral m_peripheral;
};

class ServiceBLEAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
public:
    ServiceBLEAdapter(SimpleBLE::Service _service):
        m_service(_service)
    {  }

    virtual ~ServiceBLEAdapter() = default;

    inline QString path() {
        return QString::fromStdString(m_service.data());
    };

    inline QString uuid() {
        return QString::fromStdString(m_service.uuid());
    }

    Q_INVOKABLE QStringList introspectCharacterisitics();
signals:
    void uuidChanged();
    void pathChanged();

private:
   SimpleBLE::Service m_service;
};
#endif // PERIPHERALADAPTER_H
