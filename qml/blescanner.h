#ifndef BLESCANNER_H
#define BLESCANNER_H

#include <QObject>
#include <QStringListModel>
#include <QTimer>
//nado sobrat libu
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

class BLEScanner : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringListModel* scanResults READ scanResults CONSTANT)

public:
    explicit BLEScanner(QObject *parent = nullptr);
    ~BLEScanner();

    QStringListModel* scanResults() { return &m_scanResults; }

public slots:
    void startScanning();
    void stopScanning();

private slots:
    void scan();

private:
    QTimer m_timer;
    QStringListModel m_scanResults;
    int m_hciSocket;
    int m_devId;
};

#endif // BLESCANNER_H
