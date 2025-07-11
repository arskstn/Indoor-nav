#include "blescanner.h"
#include <QDebug>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

BLEScanner::BLEScanner(QObject *parent) : QObject(parent), m_hciSocket(-1), m_devId(-1) {
    m_devId = hci_get_route(nullptr);
    if (m_devId < 0) {
        qWarning() << "bt adapter kaput";
        return;
    }

    m_hciSocket = hci_open_dev(m_devId);
    if (m_hciSocket < 0) {
        qWarning() << "hci kaput";
        return;
    }

    connect(&m_timer, &QTimer::timeout, this, &BLEScanner::scan);
    m_timer.setInterval(1000);
}

BLEScanner::~BLEScanner() {
    stopScanning();
    if (m_hciSocket >= 0) {
        hci_close_dev(m_hciSocket);
    }
}

void BLEScanner::startScanning() {
    if (m_hciSocket < 0) {
        qWarning() << "hci init kaput";
        return;
    }

    struct hci_le_set_scan_parameters_cp scan_params = {
        .type = 0x01,
        .interval = htobs(0x0010),
        .window = htobs(0x0010),
        .own_type = 0x00,
        .filter = 0x00
    };

    struct hci_request rq = {0};
    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_SCAN_PARAMETERS;
    rq.cparam = &scan_params;
    rq.clen = HCI_LE_SET_SCAN_PARAMETERS_CP_SIZE;
    rq.rparam = nullptr;
    rq.rlen = 0;

    if (hci_send_req(m_hciSocket, &rq, 1000) < 0) {
        qWarning() << "scan kaput";
        return;
    }

    uint8_t enable = 0x01;
    struct hci_le_set_scan_enable_cp scan_enable = {
        .enable = enable,
        .filter_dup = 0x00
    };

    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_SCAN_ENABLE;
    rq.cparam = &scan_enable;
    rq.clen = HCI_LE_SET_SCAN_ENABLE_CP_SIZE;

    if (hci_send_req(m_hciSocket, &rq, 1000) < 0) {
        qWarning() << "scan fail";
        return;
    }

    m_timer.start();
    qDebug() << "scan ok";
}

void BLEScanner::stopScanning() {
    if (m_hciSocket < 0) {
        return;
    }

    uint8_t disable = 0x00;
    struct hci_le_set_scan_enable_cp scan_enable = {
        .enable = disable,
        .filter_dup = 0x00
    };

    struct hci_request rq = {0};
    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_SCAN_ENABLE;
    rq.cparam = &scan_enable;
    rq.clen = HCI_LE_SET_SCAN_ENABLE_CP_SIZE;

    if (hci_send_req(m_hciSocket, &rq, 1000) < 0) {
        qWarning() << "scan off kaput";
    }

    m_timer.stop();
    qDebug() << "scan stop ok";
}

void BLEScanner::scan() {
    if (m_hciSocket < 0) {
        return;
    }

    QStringList results;

    uint8_t buf[HCI_MAX_EVENT_SIZE];
    int len = read(m_hciSocket, buf, sizeof(buf));
    if (len < 0) {
        qWarning() << "hci event kaput";
        return;
    }

    evt_le_meta_event *meta = (evt_le_meta_event *)(buf + (1 + HCI_EVENT_HDR_SIZE));
    if (meta->subevent != 0x02) {
        return;
    }

    le_advertising_info *info = (le_advertising_info *)(meta->data + 1);
    char addr[18];
    ba2str(&info->bdaddr, addr);
    QString deviceAddr = QString::fromLatin1(addr);

    QString deviceName = "???";
    if (info->length > 0) {
        for (int i = 0; i < info->length - 1; i++) {
            if (info->data[i] == 0x09) {
                int name_len = info->data[i - 1];
                deviceName = QString::fromUtf8((const char *)(info->data + i + 1), name_len);
                break;
            }
        }
    }

    results << QString("%1: %2").arg(deviceAddr, deviceName);
    m_scanResults.setStringList(results);

    int flags = fcntl(m_hciSocket, F_GETFL, 0);
    fcntl(m_hciSocket, F_SETFL, flags | O_NONBLOCK);
}
