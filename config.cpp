#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "config.h"

Config::Config() :
    m_video_path_default("rtsp://192.168.0.206:8080/video/h264"),
    m_yolo_cfg_path_default("/Yolo/COCO/yolov3.cfg"),
    m_yolo_names_path_default("/Yolo/COCO/coco.names"),
    m_yolo_weigths_path_default("/Yolo/COCO/yolov3.weights")
{}

void Config::read()
{
    qDebug() <<  QDir::current();
    QFile file;

    if (QFile::exists(QDir::current().path() + "/"+ "config.json"))
    {
        qDebug() << "Read saved config";

        file.setFileName("config.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QString val = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        file.close();
        QJsonObject obj = doc.object();

        m_video_path = obj["video"].toString();
        m_yolo_weigths_path = obj["weights"].toString();
        m_yolo_cfg_path = obj["cfg"].toString();
        m_yolo_names_path = obj["names"].toString();
    }
    else
    {
        qDebug() << "Create new config";

        QJsonDocument doc;

        QDir current = QDir::current();

        current.mkpath(QDir::currentPath() + "Yolo/COCO/");
        file.setFileName("config.json");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QJsonObject obj;
        obj["video"] = m_video_path_default;
        obj["weights"] = QDir::currentPath() + m_yolo_weigths_path_default;
        obj["cfg"] = QDir::currentPath() +  m_yolo_cfg_path_default;
        obj["names"] = QDir::currentPath() + m_yolo_names_path_default;

        m_video_path = m_video_path_default;
        m_yolo_weigths_path =QDir::currentPath() + m_yolo_weigths_path_default;
        m_yolo_cfg_path = QDir::currentPath() +  m_yolo_cfg_path_default;
        m_yolo_names_path = QDir::currentPath() + m_yolo_names_path_default;

        doc.setObject(obj);

     // Write json to the file as text and close the file.
        file.write(doc.toJson());
        file.close();
    }
}

void Config::save()
{
    QFile file;
    QJsonDocument doc;
    file.setFileName("config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate);
    QJsonObject obj;
    obj["video"] = m_video_path_default;
    obj["weights"] = m_yolo_weigths_path_default;
    obj["cfg"] = m_yolo_cfg_path_default;
    obj["names"] = m_yolo_names_path_default;

    doc.setObject(obj);

 // Write json to the file as text and close the file.
    file.write(doc.toJson());
    file.close();
}


QString Config::get_video_path() const
{
    qDebug() << "Video path: " << m_video_path;
    return m_video_path;
}


QString Config::get_yolo_cfg_path() const
{
    qDebug() << "Cfg path: " << m_yolo_cfg_path;
    return m_yolo_cfg_path;
}

QString Config::get_yolo_weights() const
{
    qDebug() << "Weights path: " << m_yolo_weigths_path;
    return m_yolo_weigths_path;
}

QString Config::get_yolo_names_path() const
{
    qDebug() << "Names path: " << m_yolo_names_path;
    return m_yolo_names_path;
}
