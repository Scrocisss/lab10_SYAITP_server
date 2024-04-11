
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "server.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Server server;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("server", &server);
    engine.load(QUrl(QStringLiteral("qrc:/server/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
