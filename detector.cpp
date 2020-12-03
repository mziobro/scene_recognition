#include <fstream>
#include <iostream>
#include <string>
#include <QDebug>

#include <opencv2/highgui.hpp>
#include <opencv2/dnn/dnn.hpp>

#include "detector.h"

extern std::vector<Bag> rooms;


Detector::Detector(QString cfg, QString weights, QString names)
    : m_yolo3_weights(weights),
      m_yolo3_config(cfg),
      m_yolo3_names(names)
{
    loadModel();
}

void Detector::set_config(QString cfg)
{
    m_yolo3_config = cfg;
}

void Detector::set_weights(QString weights)
{
   m_yolo3_weights = weights;
}

void Detector::set_names(QString names)
{
    m_yolo3_names = names;
}

/*!
 * \brief Detector::recognize Recognizes a single frame using openCV dnn and YOLOv3 model
 * \param frame
 * \return
 */
Bag Detector::recognize(cv::Mat frame)
{
    cv::Mat blob;
    std::vector<int> foundClasses;

    cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(m_inpWidth, m_inpHeight), cv::Scalar(0,0,0), true, false);
    m_net.setInput(blob);

    std::vector<cv::Mat> outs;
    m_net.forward(outs, getOutputsNames());
    foundClasses = postProcess(frame, outs);

    Bag referenceBag("ref");

    //Reference bag counts how many objects are in currently processed frame.
    for (auto object : foundClasses)
    {
        referenceBag.addWord(object);
    }

    return referenceBag;
}

/*!
 * \brief Detector::loadModel Initializes DNN model.
 */
void Detector::loadModel()
{

    // Reads classes names from file
    std::ifstream ifs(m_yolo3_names.toStdString().c_str());
    std::string line;
    while (std::getline(ifs, line)) m_classes.push_back(line);

    // Load the network
    m_net = cv::dnn::readNetFromDarknet(m_yolo3_config.toStdString(), m_yolo3_weights.toStdString());
    // Set computation backend
    m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    // Set processing unit
    m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

std::vector<std::string> Detector::getOutputsNames()
{
    static std::vector<std::string> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        std::vector<int> outLayers = m_net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        std::vector<std::string> layersNames = m_net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

std::vector<int> Detector::postProcess(cv::Mat &frame, const std::vector<cv::Mat> &outs)
{
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, nullptr, &confidence, nullptr, &classIdPoint);
            if (confidence > m_confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, m_confThreshold, m_nmsThreshold, indices);

    if (indices.empty())
        qDebug() << "no recognition found";
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], box.x, box.y,
                 box.x + box.width, box.y + box.height, frame);
    }

    return classIds;
}
void Detector::drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame)
{
    //Draw a rectangle displaying the bounding box
    rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(255, 178, 50), 3);

    //Get the label for the class name and its confidence
    std::string label = cv::format("%.2f", conf);
    if (!m_classes.empty())
    {
        CV_Assert(classId < (int)m_classes.size());
        label = m_classes[classId] + ":" + label;
    }


    qDebug() << "Label: " << QString::fromStdString(label);

    //Display the label at the top of the bounding box
//    int baseLine;
//    cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
//    top = cv::max(top, labelSize.height);
//    rectangle(frame, cv::Point(left, top - round(1.5*labelSize.height)), cv::Point(left + round(1.5*labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
    //    putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,0),1);
}

int Detector::get_number_of_classes()
{
    return m_classes.size();
}
