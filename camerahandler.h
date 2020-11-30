#ifndef CameraHandler_H
#define CameraHandler_H

#include <QObject>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QReadWriteLock>
#include "calibrator.h"
#include "bag.h"
#include <vector>

class CameraHandler : public QObject
{
    Q_OBJECT
public:
    CameraHandler();

    void read_camera();
    Q_INVOKABLE void run();
    Q_INVOKABLE void next();

    void set_rooms(std::vector<Bag*> rooms) { m_rooms = rooms; }
    void set_detector(Detector * det){ m_detector = det; }
    Q_INVOKABLE QString recognize();

public slots:
    void start_recording(QString filename);
    void stop_recording();
    void write_to_file();



signals:
    void new_frame(cv::Mat& frame);
    void newFrame();
    void new_desc(QString room);

private:
    cv::Mat m_frame;
    cv::VideoCapture m_camera;
    QReadWriteLock m_locker;
    QString m_filename;

    int m_frame_width;
    int m_frame_height;

    bool m_video_recording;
    QString m_source;
    cv::VideoWriter m_video;

    std::vector<Bag*> m_rooms;
    Detector *m_detector;
};

#endif // CameraHandler_H
