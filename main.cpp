#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "brewerymanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register BreweryManager so QML can use it
    qmlRegisterType<BreweryManager>("BreweryFinder", 1, 0, "BreweryManager");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("BreweryFinder", "Main");

    return app.exec();
}
