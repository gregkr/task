#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include "fieldmodel.h"
#include "imageprovider.h"
#include "statisticmodel.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    engine.addImageProvider(QLatin1String("flies"), new ImageProvider() );

    qmlRegisterType<FieldModel>("FieldModel", 1, 0, "FieldModel");
    qmlRegisterType<StatisticModel>("StatisticModel", 1, 0, "StatisticModel");

    engine.load(url);
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
