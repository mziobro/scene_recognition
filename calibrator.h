#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>
#include <opencv2/opencv.hpp>

#include "detector.h"
#include "bag.h"

/*!
 * \brief The Calibrator class Klasa kalibrująca
 * \details Ta klasa jest odpowiedzialna za proces kalibracji.
 * Po nagraniu sekwencji kalibracyjnych, są one kolejno analizowane w
 * funkcji read_filelist(), która czyta z poszczególnych plików i wysyła
 * je do funkcji calibrate_video_file(), gdzie wideo jest analizowane ramka
 * po ramce, a obiekty dodawane są do Bag dla danego pomieszczenia.
 *
 * Interfejsem klasy jest funkcja calibrate(), która jest wywoływana z QMLa.
 */
class Calibrator : public QObject
{
    Q_OBJECT
public:

    Calibrator(Detector * detector);
    void read_filelist();
    void calibrate_video_file(QString filename, QString);
    void recognize();

    void readCalibResults();

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
    std::vector<Bag *> m_rooms;
};


#endif // CALIBRATOR_H
