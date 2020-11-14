#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtConcurrent/QtConcurrent>
#include <QQmlContext>

#include "customvideoreader.h"
#include "customvideowriter.h"
#include "detector.h"
#include "bag.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    CustomVideoReader vid;


    //Detector* detector = new Detector("C:/Users/user/Desktop/BIOMED/pracaMagisterska/Kod/yolo3_labelled_data_train.cfg", "C:/Users/user/Desktop/BIOMED/pracaMagisterska/Kod/yolo3_labelled_data_train_5000.weights");
    Detector* detector = new Detector("C:/Users/user/Desktop/BIOMED/pracaMagisterska/Deep learning/YOLO&opencv/\
                                                    Activity-2/YOLO-3-OpenCV/yolo-coco-data/yolov3.cfg", \
                                      "C:/Users/user/Desktop/BIOMED/pracaMagisterska/Deep learning/YOLO&opencv/\
                                                    Activity-2/YOLO-3-OpenCV/yolo-coco-data/yolov3.weights");


    CustomVideoWriter videoWriter(detector);


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty("videoWriter", &videoWriter);
    engine.addImageProvider("openCVimageCapture", &vid);
    engine.load(url);


    return app.exec();
}
