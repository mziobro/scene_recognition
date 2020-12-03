#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <QStringList>

#include "bag.h"

#include <QObject>

/*!
 * \brief The Detector class
 * \details Klasa obsługująca sieć neuronową. Wykorzystuje funkcje
 * biblioteki deep neural network z OpenCV. Model ładowany jest przez
 * funkcję loadModel(). Detekcja odbywa się w funckji recognize(), która zwraca
 * Bag referencyjnym czyli rozpoznane obiekty i ich liczebność.
 */
class Detector : QObject
{
    Q_OBJECT
public:
    enum class DetectType { Calibrate, Recognize };
    explicit Detector(QString cfg, QString weights, QString names);


    Q_INVOKABLE void set_config(QString);
    Q_INVOKABLE void set_weights(QString);
    Q_INVOKABLE void set_names(QString);
    Q_INVOKABLE void loadModel();


    Bag recognize(cv::Mat frame);
    std::vector<std::string> getOutputsNames();
    std::vector<int> postProcess(cv::Mat& frame, const std::vector<cv::Mat>& out);
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) ;

    int get_number_of_classes();

private:

    QStringList m_objects;
    QString m_yolo3_weights;
    QString m_yolo3_config;
    QString m_yolo3_names;

    cv::dnn::Net m_net;

    //image parameters
    const float m_confThreshold = 0.5f;
    const float m_nmsThreshold = 0.4f;
    const int m_inpWidth = 416;
    const int m_inpHeight = 416;

    std::vector<std::string> m_classes;
};

#endif // DETECTOR_H
