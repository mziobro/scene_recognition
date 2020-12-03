#ifndef CUSTOMVIDEOREADER_H
#define CUSTOMVIDEOREADER_H

#include <QImage>
#include <QQuickImageProvider>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QReadWriteLock>

/*!
 * \brief The VideoImageProvider class
 * \details klasa - wtyczka do QMLa. Aktualizuje
 * obrazki zarówno w wikoku Kalibracja jak i w widoku Aplikacja.
 */
class VideoImageProvider : public QQuickImageProvider
{
public:
    VideoImageProvider();
    Q_INVOKABLE  QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

   void set_frame(cv::Mat& frame);

private:
    QImage m_image;
    cv::Mat m_frame;
    QReadWriteLock m_locker;
};

#endif // CUSTOMVIDEOREADER_H
