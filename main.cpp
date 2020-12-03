#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtConcurrent/QtConcurrent>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


#include "videoimageprovider.h"
#include "calibrator.h"
#include "detector.h"
#include "bag.h"
#include "CameraHandler.h"
#include "calibrator.h"
#include "config.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    Config config;
    config.read();

    VideoImageProvider vid;

    Detector * detector = new Detector(config.get_yolo_cfg_path(), config.get_yolo_weights(),
                                       config.get_yolo_names_path());

    Calibrator calibrator(detector);
    CameraHandler camera(config.get_video_path());

    QObject::connect(&camera, &CameraHandler::new_frame,[&vid](cv::Mat frame){
        vid.set_frame(frame);
    });

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty("videoWriter", &camera);
    engine.rootContext()->setContextProperty("calibrator", &calibrator);


    engine.addImageProvider("openCVimageCapture", &vid);
    engine.load(url);

    camera.set_detector(detector);


    calibrator.readCalibResults();
    camera.set_rooms(calibrator.get_rooms());

    camera.read_frame();

    return app.exec();
}
