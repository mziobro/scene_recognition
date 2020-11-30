#include "calibrator.h"

#include <QDebug>
#include <QtConcurrent/QtConcurrent>

Calibrator::Calibrator(Detector * detector)
    : m_detector(detector)
{}

void Calibrator::read_filelist()
{
    QDir workspace_dir;

    QStringList list { workspace_dir.entryList(QDir::Files)};

    for (auto entry : list)
    {
        if (entry.contains(".avi"))
        {
            QString name = entry.split(".").first();
            calibrate_video_file(entry, name);
        }
    }
}

void Calibrator::calibrate_video_file(QString filename, QString name)
{
    bool isNew = true;
    Bag * bagToWrite = new Bag(name);

    for (auto &bag : m_rooms)
    {
        if (bag->getName() == name)
        {
            //there already is a bag for this room!
            isNew = false;
            bag->clear();
            bagToWrite = bag;
            break;
        }
    }

        if(isNew)
        {
            qDebug() << "Hey, this is a new room!";
            m_rooms.push_back(bagToWrite);
        }

        cv::VideoCapture videoFile(filename.toStdString());

        if(!videoFile.isOpened())
        {
            qDebug() << "Error opening video stream: " + filename;
            return;
        }

        while(videoFile.isOpened())
        {
            videoFile >> m_frame;
            if (m_frame.empty())
                break;


            Bag referenceBag("refBag");
                referenceBag =  m_detector->recognize(m_frame);
            bagToWrite->merge(referenceBag);
        }

        bagToWrite->save();
}

void Calibrator::calibrate()
{
    read_filelist();


    calibrate_weights();
    emit calibrationEnd();
}

void Calibrator::calibrate_weights()
{
    qDebug() << "Calibrate some weights!";
    int classes = m_detector->get_number_of_classes();
    int max = 0;
    int temp = 0;
    for (int i = 0; i < classes; i++)
    {
        max = 0;
        //Find max value
        for (unsigned int j = 0; j < m_rooms.size(); j++ )
        {
            if (m_rooms.at(j)->getOccurance(i)  > max )
                max = m_rooms.at(j)->getOccurance(i);
        }

        for (unsigned int j = 0; j < m_rooms.size(); j++ )
        {
            temp = m_rooms.at(j)->getOccurance(i);
            if (temp > 0 && max != 0)
            {
                m_rooms.at(j)->set_weight(float(temp)/float(max), i);
            }
        }
    }

    for (auto& room : m_rooms)
    {
        room->save();
    }
}
