// SPDX-FileCopyrightText: Copyright 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "adaptercheck.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMessage>
#include <QDebug>
#include <QDBusVariant>

AdapterCheck::AdapterCheck(QObject *parent) : QThread(parent)
{
}

QString AdapterCheck::adapterStatusPage() const
{
    return m_statusPage;
}

void AdapterCheck::run() {
    QDBusInterface adapterInterface(SERVICE_NAME, SERVICE_PATH, INTERFACE_NAME, QDBusConnection::systemBus());
    QDBusReply<QDBusVariant> reply = adapterInterface.call("Get","org.bluez.Adapter1", "Powered");

    if (reply.isValid()) {
        m_statusPage = "pages/MainPage.qml";
        setPowered(reply.value().variant().toBool());

        if (QDBusConnection::systemBus().connect(SERVICE_NAME, SERVICE_PATH, INTERFACE_NAME, "PropertiesChanged",
                                                 this, SLOT(propertyChanged(QString, QVariantMap, QStringList)))) {
            qDebug() << "PropertiesChanged signal connected successfully to slot";
        } else {
            qDebug() << "PropertiesChanged signal connection was not successful";
        }
    } else {
        m_statusPage = "pages/ErrorPage.qml";
        qDebug() << reply.error();
    }
    emit adapterStatusPageChanged();
}

bool AdapterCheck::powered()
{
    return m_powered;
}

void AdapterCheck::propertyChanged(QString name, QVariantMap map, QStringList list)
{
    Q_UNUSED(list);
    Q_UNUSED(name);

    if(map.contains("Powered")) {
       setPowered(map["Powered"].toBool());
    }
}

void AdapterCheck::setPowered(bool newPowered)
{
    if (m_powered == newPowered)
        return;
    m_powered = newPowered;
    emit poweredChanged();
}
