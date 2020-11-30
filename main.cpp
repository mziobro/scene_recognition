#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtConcurrent/QtConcurrent>
#include <QQmlContext>

#include "videoimageprovider.h"
#include "calibrator.h"
#include "detector.h"
#include "bag.h"
#include "CameraHandler.h"
#include "calibrator.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    VideoImageProvider vid;


//    Detector* detector = new Detector("C:/Users/user/Desktop/BIOMED/pracaMagisterska/Deep learning/YOLO&opencv/Activity-2/YOLO-3-OpenCV/yolo-coco-data/yolov3.cfg",
//                                      "C:/Users/user/Desktop/BIOMED/pracaMagisterska/Deep learning/YOLO&opencv/Activity-2/YOLO-3-OpenCV/yolo-coco-data/yolov3.weights");

    Detector * detector = new Detector("C:/Users/user/Desktop/BIOMED/pracaMagisterska/Baza_danych/yolo3_labelled_data_test.cfg",
                                       "C:/Users/user/Desktop/BIOMED/pracaMagisterska/Baza_danych/yolo3_labelled_data_train_21900.weights");

    Calibrator calibrator(detector);

    CameraHandler camera;

//    camera.run();

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

    Bag * lazienka = new Bag("Łazienka");
    Bag * salon = new Bag("Salon");
    Bag * kuchnia = new Bag("Kuchnia");
    Bag * przedpokoj = new Bag("Przedpokój");
    Bag * sypialnia = new Bag("Sypialnia");

    //COCO
    lazienka->addWord(71, 2);
    lazienka->addWord(61, 2);
    lazienka->addWord(39, 4);
    lazienka->addWord(68, 2);
    lazienka->addWord(0, 1);
    lazienka->addWord(41, 1);
    lazienka->addWord(72, 1);
    lazienka->addWord(79, 1);
    lazienka->addWord(15, 1);

    salon->addWord(63, 2);
    salon->addWord(56, 6);
    salon->addWord(41, 2);
    salon->addWord(45, 1);
    salon->addWord(60, 1);
    salon->addWord(57, 1);
    salon->addWord(39, 1);
    salon->addWord(28, 1);
    salon->addWord(75, 1);
    salon->addWord(79, 1);
    salon->addWord(62, 1);
    salon->addWord(66, 2);
    salon->addWord(74, 1);
    salon->addWord(59, 1);
    salon->addWord(58, 1);
    salon->addWord(64, 2);
    salon->addWord(40, 2);
    salon->addWord(73, 14);
    salon->addWord(15, 1);
    salon->addWord(71, 1);
    salon->addWord(24, 1);
    salon->addWord(42, 1);
    salon->addWord(68, 1);
    salon->addWord(0, 1);
    salon->addWord(72, 1);

    kuchnia->addWord(68, 2);
    kuchnia->addWord(39, 6);
    kuchnia->addWord(69, 2);
    kuchnia->addWord(41, 2);
    kuchnia->addWord(71, 3);
    kuchnia->addWord(45, 2);
    kuchnia->addWord(43, 2);
    kuchnia->addWord(71, 1);
    kuchnia->addWord(0, 1);
    kuchnia->addWord(56, 1);
    kuchnia->addWord(73, 3);
    kuchnia->addWord(58, 1);

    sypialnia->addWord(0, 1);
    sypialnia->addWord(59, 1);
    sypialnia->addWord(15, 1);
    sypialnia->addWord(58, 1);
    sypialnia->addWord(56, 1);
    sypialnia->addWord(72, 1);


    przedpokoj->addWord(39, 1);
    przedpokoj->addWord(58, 1);
    przedpokoj->addWord(0, 1);
    przedpokoj->addWord(63, 1);
    przedpokoj->addWord(75, 1);
    przedpokoj->addWord(45, 1);


    //own
//    salon->addWord(3,1);
//    salon->addWord(0,1);
//    salon->addWord(11,1);
//    salon->addWord(12,1);
//    salon->addWord(6,1);
//    salon->addWord(5,2);
//    salon->addWord(8,1);

//    kuchnia->addWord(15, 1);
//    kuchnia->addWord(3, 1);
//    kuchnia->addWord(12, 1);
//    kuchnia->addWord(11, 1);
//    kuchnia->addWord(17, 1);
//    kuchnia->addWord(5, 1);

//    sypialnia->addWord(11,1);
//    sypialnia->addWord(0,1);
//    sypialnia->addWord(3,1);
//    sypialnia->addWord(12,1);
//    sypialnia->addWord(17,1);

//    lazienka->addWord(6,1);
//    lazienka->addWord(5,2);
//    lazienka->addWord(12,1);
//    lazienka->addWord(17,1);
//    lazienka->addWord(3,1);
//    lazienka->addWord(18,1);
//    lazienka->addWord(1,1);
//    lazienka->addWord(15,1);

//    przedpokoj->addWord(3,1);
//    przedpokoj->addWord(12,1);
//    przedpokoj->addWord(5,1);
//    przedpokoj->addWord(18,1);
//    przedpokoj->addWord(17,1);
//    przedpokoj->addWord(16,1);
//    przedpokoj->addWord(0,1);

    std::vector<Bag*> rooms;

    rooms.push_back(przedpokoj);
    rooms.push_back(lazienka);
    rooms.push_back(kuchnia);
    rooms.push_back(salon);
    rooms.push_back(sypialnia);

    camera.set_rooms(rooms);
    calibrator.set_rooms(rooms);

    calibrator.calibrate_weights();

    return app.exec();
}
