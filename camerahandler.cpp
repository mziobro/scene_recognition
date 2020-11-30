#include "camerahandler.h"

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QReadWriteLock>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

CameraHandler::CameraHandler() :
    m_video_recording(false),
    m_source("C:/Users/user/Documents/build-Master_App-Desktop_Qt_5_9_8_MinGW_32bit-Debug/Pomieszczenie.avi"),//"rtsp://192.168.0.206:8080/video/h264"),
    m_filename("pomieszczenie")
{
    m_camera.open(m_source.toStdString());

    m_frame_width = m_camera.get(cv::CAP_PROP_FRAME_WIDTH);
    m_frame_height = m_camera.get(cv::CAP_PROP_FRAME_HEIGHT);

    if(!m_camera.isOpened())
        qDebug() << "Cannot open camera!";
    else qDebug() << "Camera opened";

    //    read_camera();
}

void CameraHandler::read_camera()
{
    m_camera >> m_frame;

    if (m_frame.empty())
    {
        //        qDebug() << "Frame is empty!";
        return;
    }
    else {
        QString name = recognize();
        emit new_frame(m_frame);
        emit newFrame();
        emit new_desc(name);
    }
}

void CameraHandler::run()
{
    QtConcurrent::run([this](){

        if(m_camera.isOpened())
        {
            while (true)
            {
                QReadLocker locker(&m_locker);
                read_camera();

                if (m_video_recording)
                    write_to_file();
            }
        }

    });
}


void CameraHandler::next()
{
    QtConcurrent::run([this](){

        if(m_camera.isOpened())
        {
            QReadLocker locker(&m_locker);
            read_camera();
        }
    });
}

QString CameraHandler::recognize()
{
    qDebug() << "recognizing";
    Bag referenceBag("refBag");

    // If the frame is empty, break immediately
    if (m_frame.empty())
        return QString();

    referenceBag = m_detector->recognize(m_frame);

    std::vector<float> points;
    int numberOfRooms = 0;
    uint8_t room_index = -1;
    bool atLeastOneMatches = false;

    //If there is at least one room
    if(!m_rooms.empty())
    {
        if (!referenceBag.m_bagOfwords.empty())
        {
            //Iterate through each object in
            for (auto referenceWord : referenceBag.m_bagOfwords)
            {
                //Iterate through all mapped rooms
                for(uint8_t i = 0; i < m_rooms.size(); ++i)
                {
                    //Initialize room points to 0
                    points.push_back(0.0f);

                    //Iterate through every object in current room
                    for(auto word : m_rooms.at(i)->m_bagOfwords)
                    {
                        if(word.idx == referenceWord.idx)
                        {
                            numberOfRooms++;
                            room_index = i;
                            points.back() += word.weight;
                            atLeastOneMatches = true;
                        }
                    }
                }
            }

            if (!atLeastOneMatches)
                return QString("Nie oznaczono żadnego\n"
                               "pomieszczenia");

            auto it = std::max_element(points.begin(), points.end());
            int index = std::distance(points.begin(), it);

            return m_rooms.at(index)->getName();

        }

        return QString("Nie oznaczono żadnego\n"
                       "pomieszczenia");
    }
}

void CameraHandler::write_to_file()
{
    m_video.write(m_frame);
}

void CameraHandler::start_recording(QString filename)
{
    QtConcurrent::run([this, filename](){
        QReadLocker locker(&m_locker);

        m_filename = filename + ".avi";
        m_video_recording = true;
        m_video.open(m_filename.toStdString(),cv::VideoWriter::fourcc('M','J','P','G'),
                     10, cv::Size(m_frame_width,m_frame_height));});

}

void CameraHandler::stop_recording()
{
    m_video_recording = false;
    m_video.release();
}