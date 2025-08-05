// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef BLESCANMODEL_H
#define BLESCANMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QStringList>
#include <simpleble/SimpleBLE.h>
#include <simpledbus/base/Exceptions.h>
#include "peripheraladapter.h"
#include "basemodel.h"

class BleScanModel : public QAbstractListModel, public BaseModel<PeripheralAdapter>
{
    Q_OBJECT

    Q_PROPERTY(bool discovering READ discovering WRITE setDiscovering NOTIFY discoveringChanged)
    Q_PROPERTY(bool powered WRITE setPowered)
    Q_PROPERTY(QString diagnosticMessage READ diagnosticMessage NOTIFY diagnosticMessageChanged)
    Q_PROPERTY(QString scanInfo READ scanInfo NOTIFY scanInfoChanged)
    Q_PROPERTY(bool sortByNameEnabled READ sortByNameEnabled WRITE setSortByNameEnabled NOTIFY sortByNameEnabledChanged)
    Q_PROPERTY(QStringList gridData READ gridData NOTIFY gridDataChanged)
    Q_PROPERTY(QPointF position READ position NOTIFY positionChanged)
    Q_INVOKABLE void updateDiagnosticMessage();

public:
    explicit BleScanModel(QObject *parent = nullptr);
    QPointF position() const;
    QVariant data(const QModelIndex &index, int role) const override;
    enum RoleNames {
        NameRole = Qt::UserRole + 1,
        AddressRole,
        AddressTypeRole,
        RssiRole,
        PairedRole,
        ConnectedRole
    };
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    bool discovering() override;
    void setDiscovering(bool discovering) override;
    Q_INVOKABLE void connectToDevice(int index) override;
    Q_INVOKABLE void disconnectFromDevice(int index) override;
    Q_INVOKABLE QList<QObject*> introspectService(int index) override;
    Q_INVOKABLE void clear() override;
    Q_INVOKABLE void startContinuousScan();
    Q_INVOKABLE void stopContinuousScan();
    Q_INVOKABLE void clearScanInfo();
    Q_INVOKABLE void clearFilteredDevices();
    Q_INVOKABLE void sortByName(bool enabled);
    void setPowered(bool value);
    QStringList gridData() const { return m_gridData; }
    Q_INVOKABLE void loadGridData();
    Q_INVOKABLE void calculateTriangulation();
    Q_INVOKABLE void calculateNearestNeighbor();

    QString diagnosticMessage() const { return m_diagnosticMessage; }
    QString scanInfo() const { return m_scanInfo; }
    bool sortByNameEnabled() const { return m_sortByNameEnabled; }
    void setSortByNameEnabled(bool enabled);
    QPoint positionInPixels() const;

private slots:
    void updateDeviceInfo(SimpleBLE::Peripheral peripheral);
    void searchDevice(SimpleBLE::Peripheral peripheral);
    void updateScanInfo();

signals:
    void discoveringChanged();
    void poweredChanged();
    void diagnosticMessageChanged();
    void scanInfoChanged();
    void sortByNameEnabledChanged();
    void gridDataChanged();
    void positionChanged();

private:
    void init();
    void updateFilteredDevices();
    SimpleBLE::Adapter m_adapter;
    QTimer m_scanInfoTimer;
    QString m_diagnosticMessage;
    QString m_scanInfo;
    bool m_sortByNameEnabled = false;
    QList<std::shared_ptr<PeripheralAdapter>> m_filteredDevices;
    QStringList m_gridData;
    QMap<QString, QList<int>> m_rssiHistory;
    QPointF m_position;
    void updatePosition(const QPointF &pos);
    double calculateDistance(int rssi) const;
    QPointF triangulatePosition() const;
    QPointF nearestNeighborPosition() const;
};
#endif // BLESCANMODEL_H
