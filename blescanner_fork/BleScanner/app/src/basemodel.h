// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <memory>

template<typename DeviceType>
class BaseModel
{

public:
    explicit BaseModel(){};

    enum ObjectRoles
    {
        NameRole = Qt::UserRole + 1,
        AddressRole,
        AddressTypeRole,
        RssiRole,
        PairedRole,
        ConnectedRole
    };

    QHash<int, QByteArray> roleNames() const{
        static const QHash<int, QByteArray> roles {
            {NameRole, "name"},
            {AddressRole, "address"},
            {AddressTypeRole, "addressType"},
            {RssiRole, "rssi"},
            {PairedRole, "paired"},
            {ConnectedRole, "connected"}
        };
        return roles;
    }
    int rowCount(const QModelIndex &parent) const{
        Q_UNUSED(parent);
        return m_listDevices.size();
    }

    virtual bool discovering() = 0;
    virtual void setDiscovering(bool discovering) = 0;

    virtual void connectToDevice(int index) = 0;
    virtual void disconnectFromDevice(int index) = 0;
    virtual QList<QObject*> introspectService(int index) = 0;
    virtual void clear(){
        if(!m_adapterPower) {
            setDiscovering(false);
        }
        if(discovering()) {
            setDiscovering(false);
        }
        m_listDevices.clear();
    };

protected:
    QList<std::shared_ptr<DeviceType>> m_listDevices;
    bool m_adapterPower = false;
};

#endif // BASEMODEL_H
