#include "videoimageprovider.h"
#include "CameraHandler.h"

#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>


VideoImageProvider::VideoImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading),
      m_image(QImage(50,50, QImage::Format_RGB32))
{
    m_image.fill(Qt::white);
}

//VideoImageProvider::~VideoImageProvider()
//{}

QImage VideoImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id)
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)

   if(!m_frame.empty()){
        cv::cvtColor(m_frame, m_frame, cv::COLOR_BGR2RGB);
        m_image = QImage(static_cast<uchar*>(m_frame.data), m_frame.cols, m_frame.rows, m_frame.step1(), QImage::Format_RGB888);
   }
   else
       qDebug() << "No frame yet";
    return m_image;
}

void VideoImageProvider::set_frame(cv::Mat& frame)
{
    m_frame = frame;
}


