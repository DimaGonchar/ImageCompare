#include "src/ScreenCaptureController.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/ImageCompare/main.qml"_qs);

    ScreenCaptureController* captureController = new ScreenCaptureController(&engine);
    engine.rootContext()->setContextProperty("screenCaptureController", captureController);
    engine.rootContext()->setContextProperty("screenshotsModel", captureController->screenshotsModel().get());

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
