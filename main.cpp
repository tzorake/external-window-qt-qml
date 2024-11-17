#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "EmbeddedApplication.h"

#define _MODE 1

#if _MODE == 1

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<EmbeddedApplication>("CustomComponents", 1, 0, "EmbeddedApplication");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("EXE_PATH", "D:/Projects/external-window-qt-qml/web/dist/win-unpacked/web.exe");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

#endif
