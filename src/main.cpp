#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "database/DatabaseManager.h"
#include "auth/UserManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");

    // Initialize Database
    DatabaseManager::instance().connectToDatabase();

    // Initialize Logic
    UserManager userManager;

    QQmlApplicationEngine engine;
    
    // Expose Logic to QML
    engine.rootContext()->setContextProperty("userManager", &userManager);
    engine.rootContext()->setContextProperty("databaseManager", &DatabaseManager::instance());
    
    // Load QML from the Qt Resource SystemxtProperty("databaseManager", &DatabaseManager::instance());
    
    // Load QML from the Qt Resource System
    const QUrl url("qrc:/QtFaceAuth/ui/main.qml");
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
