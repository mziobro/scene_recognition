#include "customvideowriter.h"

#include <QDebug>
#include <QtConcurrent/QtConcurrent>

extern std::vector<Bag> rooms;

CustomVideoWriter::CustomVideoWriter(Detector * detector)
    : QObject(nullptr),
      m_source("rtsp://192.168.0.206:8080/video/h264"),
      m_videoStop(false),
      m_detector(detector)
{
    m_camera = cv::VideoCapture(m_source.toStdString());
}

void CustomVideoWriter::writeToVideoFile(QString& name)
{
    QReadLocker locker(&m_locker);
    QString outputName = name + ".avi";
    bool isNew = true;
    Bag * bagToWrite;


    for (auto &bag : m_rooms)
    {
        if (bag.getName() == name)
        {
            //there already is a bag for this room!
            isNew = false;
            bag.clear();
            bagToWrite = &bag;
            break;
        }
    }

    if(isNew)
    {
        qDebug() << "Hey, this is a new room!";
        m_rooms.push_back(name);
        bagToWrite = &(m_rooms.back());
    }

    qDebug() << "Saving a file to " << outputName;

    m_camera = cv::VideoCapture(m_source.toStdString());

    if(!m_camera.isOpened())
    {
        qDebug() << "Error opening video stream: record";
        return;
    }
    // Default resolution of the frame is obtained.The default resolution is system dependent.
    int frame_width = m_camera.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = m_camera.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::VideoWriter video(outputName.toStdString(),cv::VideoWriter::fourcc('M','J','P','G'),10, cv::Size(frame_width,frame_height));
    while(1)
    {
        // Capture frame-by-frame
        m_camera >> m_frame;
        // If the frame is empty, break immediately
        if (m_frame.empty())
            break;
        video.write(m_frame);

        m_locker.lockForRead();
        if(m_videoStop)
        {
            m_locker.unlock();
            break;
        }
        else
            m_locker.unlock();
    }
    video.release();
    m_camera.release();

    qDebug() << "Stop recording";

    cv::VideoCapture videoFile(outputName.toStdString());

    if(!videoFile.isOpened())
    {
        qDebug() << "File recorded badly";
        return;
    }
    else {
        emit startCalibrate(name);
    }

    cv::Mat frame ;
    while(videoFile.read(frame))
    {

        Bag referenceBag = m_detector->recognize(m_frame);
        bagToWrite->merge(referenceBag);

    }

    bagToWrite->save();

    m_videoStop = false;
    emit endCalibrate(name);
    qDebug() << "End of recording & analyzing";
}

void CustomVideoWriter::record(QString name)
{

    qDebug() << "Video recording starts!";
//    writeToVideoFile(name);
    QtConcurrent::run(this, &CustomVideoWriter::writeToVideoFile, name);
}


/*!
 * \brief CustomVideoWriter::recognize This function compares captured frame
 * with previously created
 * \return
 */
QString CustomVideoWriter::recognize()
{
    m_camera = cv::VideoCapture(m_source.toStdString());

    Bag referenceBag("refBag");

    if(!m_camera.isOpened())
    {
        qDebug() << "Error opening video stream: record";
        return QString();
    }

    m_camera >> m_frame;
    // If the frame is empty, break immediately
    if (m_frame.empty())
        return QString();

    referenceBag = m_detector->recognize(m_frame);

    m_camera.release();


    std::vector<int> points;
    int numberOfRooms = 0;
    uint8_t room_index = -1;


    //If there is at least one room
    if(!m_rooms.empty())
    {
        //Iterate through each object in
        for (auto referenceWord : referenceBag.m_bagOfwords)
        {
            //Iterate through all mapped rooms
            for(uint8_t i = 0; i < m_rooms.size(); ++i)
            {
                //Initialize room points to 0
                points.push_back(0);

                //Iterate through every object in current room
                for(auto word : m_rooms.at(i).m_bagOfwords)
                {
                    if(word.idx == referenceWord.idx)
                    {
                        numberOfRooms++;
                        room_index = i;
                        points.back() += word.occurances;
                    }
                }
            }

            //Current object was recognized only in one room during calibration
            if(numberOfRooms == 1)
            {
                return m_rooms.at(room_index).getName();
            }
            else {

                numberOfRooms = 0;
            }
        }

        auto it = std::max_element(points.begin(), points.end());
        int index = std::distance(points.begin(), it);

        return m_rooms.at(index).getName();

    }

    return "Nie oznaczono żadnego\n"
           "pomieszczenia";
}

void CustomVideoWriter::stop()
{
    m_videoStop = true;
    qDebug() << "videostop set to true";
}

void CustomVideoWriter::calibrate()
{

}
