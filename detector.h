#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <QStringList>

#include "bag.h"

class Detector
{
public:
    enum class DetectType { Calibrate, Recognize };


    explicit Detector(QString cfg, QString weights);

    Bag recognize(cv::Mat frame);
    std::vector<std::string> getOutputsNames();
    std::vector<int> postProcess(cv::Mat& frame, const std::vector<cv::Mat>& out);
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) ;

private:
    void loadModel();

    QStringList m_objects;
    const QString m_yolo3_weights;
    const QString m_yolo3_config;
    cv::dnn::Net m_net;

    //image parameters
    const float m_confThreshold = 0.5f;
    const float m_nmsThreshold = 0.4f;
    const int m_inpWidth = 416;
    const int m_inpHeight = 416;
    std::vector<std::string> m_classes;
};

#endif // DETECTOR_H
