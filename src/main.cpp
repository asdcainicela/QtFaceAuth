#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "database/DatabaseManager.h"
#include "auth/UserManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Initialize Database
    DatabaseManager::instance().connectToDatabase();

    // Initialize Logic
    UserManager userManager;

    QQmlApplicationEngine engine;
    
    // Expose Logic to QML
    engine.rootContext()->setContextProperty("userManager", &userManager);
    // Load QML from the new architecture path (ui/main.qml)
    // Note: In production you might want to use qrc:/ui/main.qml
    const QUrl url(QStringLiteral("ui/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
