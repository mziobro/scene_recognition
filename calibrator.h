#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>
#include <opencv2/opencv.hpp>

#include "detector.h"
#include "bag.h"

class Calibrator : public QObject
{
    Q_OBJECT
public:

    Calibrator(Detector * detector);
    void read_filelist();
    void calibrate_video_file(QString filename, QString);
    void recognize();

    std::vector<Bag*> get_rooms(){ return m_rooms; }

    void set_rooms(std::vector<Bag*> rooms){
        m_rooms = rooms;
    }

    Q_INVOKABLE void calibrate();
    void calibrate_weights();

signals:
    void calibrationEnd();
private:

    cv::Mat m_frame;
    Detector * m_detector;
    std::vector<Bag*> m_rooms;
};


#endif // CALIBRATOR_H
