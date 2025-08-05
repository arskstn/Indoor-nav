// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef BLUEZSCANMODEL_H
#define BLUEZSCANMODEL_H

#include "basemodel.h"
#include <simplebluez/Bluez.h>
#include <simplebluez/Adapter.h>
#include <QTimer>
#include <simpledbus/base/Exceptions.h>
#include <QDebug>

class BluezScanModel : public QAbstractListModel, public BaseModel<SimpleBluez::Device>
{
    Q_OBJECT

    Q_PROPERTY(bool discovering READ discovering WRITE setDiscovering NOTIFY discoveringChanged)
    Q_PROPERTY(bool powered WRITE setPowered)
    Q_PROPERTY(QString diagnosticMessage READ diagnosticMessage NOTIFY diagnosticMessageChanged)
    Q_PROPERTY(QString scanInfo READ scanInfo NOTIFY scanInfoChanged)

public:
    explicit BluezScanModel(QObject *parent = nullptr);

    enum BusType{
         Session,
         System
     };

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    bool discovering() override;
    void setDiscovering(bool discovering) override;
    Q_INVOKABLE void connectToDevice(int index) override;
    Q_INVOKABLE void disconnectFromDevice(int index) override;
    Q_INVOKABLE QList<QObject *> introspectService(int index) override;
    Q_INVOKABLE void clear() override;
    void setPowered(bool value);

    QString diagnosticMessage() const { return m_diagnosticMessage; }
    QString scanInfo() const { return m_scanInfo; }

private slots:
    void asyncRead();
    void updateDeviceInfo(std::shared_ptr<SimpleBluez::Device> device);
    void updateScanInfo();

signals:
    void discoveringChanged();
    void poweredChanged();
    void diagnosticMessageChanged();
    void scanInfoChanged();

private:
    void init();
    std::shared_ptr<SimpleBluez::Adapter> m_adapter;
    SimpleBluez::Bluez m_bluez;
    QTimer m_timer;
    QTimer m_scanInfoTimer;
    SimpleBluez::Adapter::DiscoveryFilter m_filter;
    QString m_diagnosticMessage;
    QString m_scanInfo;
};

class ServiceImp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
public:
    ServiceImp(std::shared_ptr<SimpleBluez::Service> _service):
        m_service(_service)
    { }

    Q_INVOKABLE QStringList introspectCharacterisitics();
    QString uuid();
    inline QString path() const { return QString::fromStdString(m_service->path());}
    inline bool valid() const {return m_service->valid();}
signals:
    void uuidChanged();
    void validChanged();
    void pathChanged();
private:
    std::shared_ptr<SimpleBluez::Service> m_service;
};

#endif // BLUEZSCANMODEL_H
