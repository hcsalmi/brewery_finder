#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include <QQuickWindow>
#include "brewerymanager.h"

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

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
