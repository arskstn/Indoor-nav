// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef ADAPTERCHECK_H
#define ADAPTERCHECK_H

#include <QThread>
#include <QObject>
#include <QVariantMap>

class AdapterCheck : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString adapterStatusPage READ adapterStatusPage NOTIFY adapterStatusPageChanged)
    Q_PROPERTY(bool powered READ powered NOTIFY poweredChanged)

public:
    explicit AdapterCheck(QObject *parent = nullptr);
    virtual ~AdapterCheck() = default;
    QString adapterStatusPage() const;
    void run() override;

    bool powered();

    inline static const QString SERVICE_NAME = "org.bluez";
    inline static const QString SERVICE_PATH = "/org/bluez/hci0";
    inline static const QString INTERFACE_NAME = "org.freedesktop.DBus.Properties";

    void setPowered(bool newPowered);

signals:
    void adapterStatusPageChanged();
    void poweredChanged();

private slots:
    void propertyChanged(QString name, QVariantMap map, QStringList list);

private:
    QString m_statusPage{"pages/LoadPage.qml"};
    bool m_powered = false;
};

#endif // ADAPTERCHECK_H
