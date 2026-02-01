#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "database/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Initialize Database
    DatabaseManager::instance().connectToDatabase();

    QQmlApplicationEngine engine;
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
