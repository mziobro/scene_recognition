#ifndef CameraHandler_H
#define CameraHandler_H

#include <QObject>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QReadWriteLock>
#include "calibrator.h"
#include "bag.h"
#include <vector>

/*!
 * \brief The CameraHandler class Handles camera
 * \details Klasa do obsługi streamu video. Źródło  wideo jest w
 * zasadzie obojętne, można je zmienić w pliku config.json.
 * Kamera otwierana jest w funkcji open().
 *
 * W obecnej implementacji kamera uruchamiana jest w dwóch trybach -
 * - w trakcie kalibracji funkcja run() zapewnia ciągłą pracę kamery, natomiast
 * w trakcie rzeczywistej detekcji ramki pobierane są 'na żądanie' przez funkcję
 * read_frame()
 *
 */
class CameraHandler : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool stop READ read_stop WRITE set_stop NOTIFY stopChanged)
    Q_PROPERTY(QString desc READ read_desc WRITE set_desc NOTIFY new_desc)

    CameraHandler(QString source);

    void read_camera();
    Q_INVOKABLE void read_frame();
    bool open();
    Q_INVOKABLE void run();
    Q_INVOKABLE void next();

    void set_stop (bool stop)
    {
        m_stop = stop;
        emit stopChanged();
    }

    bool read_stop() const { return m_stop; }
    QString read_desc() const { return m_description; }

    void set_rooms(std::vector<Bag*> rooms) { m_rooms = rooms; }
    void set_desc(QString text)
    {
        m_description = text;
    }

    void set_detector(Detector * det){ m_detector = det; }
    Q_INVOKABLE void recognize();
    void recognize_task();

public slots:
    void start_recording(QString filename);
    void stop_recording();
    void write_to_file();

signals:
    void new_frame(cv::Mat& frame);
    void newFrame();
    void new_desc(QString room);
    void processingDone();
    void stopChanged();
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

    bool m_stop;
    QString m_description;
};

#endif // CameraHandler_H
