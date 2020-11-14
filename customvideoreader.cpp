#include "customvideoreader.h"
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>


CustomVideoReader::CustomVideoReader()
    : QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading),
      m_image(QImage(50,50, QImage::Format_RGB32)),
      m_source("rtsp://192.168.0.206:8080/video/h264")
{
    m_image.fill(Qt::white);
//    m_camera = cv::VideoCapture("rtsp://192.168.0.206:8080/video/h264");
//    if (m_camera.isOpened() == false)
//    {
//        qDebug() << "Cannot open the video file";
//    }



}

CustomVideoReader::~CustomVideoReader()
{
    m_camera.release();
}

QImage CustomVideoReader::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id)
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)

    m_camera.open(m_source.toStdString());

    if(!m_camera.isOpened())
        qDebug() << "Cannot open camera!";
    cv::Mat frameRGB;

    m_camera >> m_frame;

    if (m_frame.empty())
    {
        qDebug() << "Frame is empty!";
        return QImage();
    }
//    m_detector->recognize(m_frame);

    qDebug() << "Frame was read";

    cv::cvtColor(m_frame, m_frame, cv::COLOR_BGR2RGB);

    m_image = QImage(static_cast<uchar*>(m_frame.data), m_frame.cols, m_frame.rows, m_frame.step1(), QImage::Format_RGB888);
    qDebug() << "Frame was passed";

    m_camera.release();

    return m_image;
}


