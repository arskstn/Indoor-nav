#include <auroraapp.h>
#include <QtQuick>
#include "blescanmodel.h"
#include "adaptercheck.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.auroraos"));
    application->setApplicationName(QStringLiteral("BLEScanner"));
    qRegisterMetaType<QPoint>("QPoint");

    qmlRegisterType<BleScanModel>("ru.omp.SimpleBLE", 1, 0, "BleScanModel");
    BleScanModel bleScanModelForMap; //MapPage
    AdapterCheck adapterStatus;
    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->rootContext()->setContextProperty(QStringLiteral("bleScanModelForMap"), &bleScanModelForMap); // Для MapPage
    view->rootContext()->setContextProperty(QStringLiteral("adapterStatus"), &adapterStatus);
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/BleScanner.qml")));
    view->show();
    adapterStatus.start();
    return application->exec();
}
