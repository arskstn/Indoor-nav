#include <auroraapp.h>
#include <QtQuick>
#include "blescanner.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.template"));
    application->setApplicationName(QStringLiteral("indoor"));

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/indoor.qml")));
    view->show();

    QGuiApplication app(argc, argv);

        // Регистрируем BleScanner для использования в QML
        qmlRegisterType<BleScanner>("com.example", 1, 0, "BleScanner");

        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;

    return application->exec();
}
