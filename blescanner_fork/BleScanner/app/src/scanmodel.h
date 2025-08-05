// SPDX-FileCopyrightText: Copyright 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SCANMODEL_H
#define SCANMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QFuture>
#include <QTimer>
#include <QThread>
#include <simpleble/SimpleBLE.h>
#include <simplebluez/Bluez.h>
#include <simplebluez/Adapter.h>
#include <simplebluez/interfaces/Device1.h>
#include <simpledbus/base/Exceptions.h>


class ScanModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool discovering READ discovering WRITE setDiscovering NOTIFY discoveringChanged)
    Q_PROPERTY(bool powered READ powered NOTIFY poweredChanged)

public:

    enum ObjectRoles
    {
        NameRole = Qt::UserRole + 1,
        AddressRole,
        AddressTypeRole,
        RssiRole,
        PairedRole,
        ConnectedRole,
        DeviceNameRole,
    };

    enum BusType{
        Session,
        System
    };

    explicit ScanModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool discovering() const;
    bool powered() const;
    void setDiscovering(bool discovering);

    Q_INVOKABLE void connectToDevice(int index);
    Q_INVOKABLE void disconnectFromDevice(int index);
    Q_INVOKABLE QList<QObject*> introspectService(int index);
    Q_INVOKABLE void clear();

private slots:
    void asyncRead();
    void updateDeviceInfo(std::shared_ptr<SimpleBluez::Device> device);

signals:
    void discoveringChanged();
    void poweredChanged();

private:
    std::shared_ptr<SimpleBluez::Adapter> m_adapter;
    SimpleBluez::Bluez m_bluez;
    QTimer m_timer;
    QMultiMap<int, std::shared_ptr<SimpleBluez::Device>> m_list;
    QList<std::shared_ptr<SimpleBluez::Device>> m_listDevices;

};

class ServiceImp: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
public:
    ServiceImp(std::shared_ptr<SimpleBluez::Service> _service):
        m_service(_service)
    {

    }
    Q_INVOKABLE QStringList introspectCharacterisitics(){
        QStringList listChar;
        for (auto characteristic : m_service->characteristics()) {
            qDebug() << QString::fromStdString(characteristic->uuid());
            listChar.append(QString::fromStdString(characteristic->uuid()));
        }
        return listChar;
    };
    QString uuid(){
        QString result;
        try {
            result = QString::fromStdString(m_service->uuid());
        }  catch (SimpleDBus::Exception::InterfaceNotFoundException& e) {
            result = QString::fromStdString(e.what());
        }
        return result;
    }
    QString path(){ return QString::fromStdString(m_service->path());}
    bool valid(){return m_service->valid();}
signals:
    void uuidChanged();
    void validChanged();
    void pathChanged();
    void listCharChanged();
private:
    std::shared_ptr<SimpleBluez::Service> m_service;
};
#endif // SCANMODEL_H
