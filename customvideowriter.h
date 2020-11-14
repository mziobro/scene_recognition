#ifndef CUSTOMVIDEOWRITER_H
#define CUSTOMVIDEOWRITER_H

#include <QObject>
#include <QReadWriteLock>
#include <opencv2/opencv.hpp>

#include "detector.h"
#include "bag.h"

class CustomVideoWriter : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool videoStop MEMBER m_videoStop)

    CustomVideoWriter(Detector * detector);
    void writeToVideoFile(QString &name);
    Q_INVOKABLE void record(QString name);
    Q_INVOKABLE QString recognize();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void calibrate();

signals:
    void startCalibrate(QString room);
    void endCalibrate(QString room);

private:
    cv::VideoCapture m_camera;
    QString m_source;
    cv::Mat m_frame;
    bool m_videoStop;
    QReadWriteLock m_locker;
    Detector * m_detector;
    std::vector<Bag> m_rooms;
};


#endif // CUSTOMVIDEOWRITER_H
