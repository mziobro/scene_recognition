#ifndef CUSTOMVIDEOREADER_H
#define CUSTOMVIDEOREADER_H

#include <QImage>
#include <QQuickImageProvider>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QReadWriteLock>

class CustomVideoReader : public QQuickImageProvider
{
public:
    CustomVideoReader();
    ~CustomVideoReader() override;
    Q_INVOKABLE  QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void readImage();

private:
    QImage m_image;
    cv::Mat m_frame;
    cv::VideoCapture m_camera;
    QString m_source;
    QReadWriteLock m_locker;
};

#endif // CUSTOMVIDEOREADER_H
