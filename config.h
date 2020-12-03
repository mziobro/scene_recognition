#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

/*!
 * \brief The Config class is responsible for storing
 * paths and other configurable settings
 */
class Config
{
public:

    Config();

    void read();
    void save();

    QString get_video_path() const;
    QString get_yolo_cfg_path() const;
    QString get_yolo_weights() const;
    QString get_yolo_names_path() const;

private:
    QString m_video_path;
    QString m_yolo_cfg_path;
    QString m_yolo_names_path;
    QString m_yolo_weigths_path;

    const QString m_video_path_default;
    const QString m_yolo_cfg_path_default;
    const QString m_yolo_names_path_default;
    const QString m_yolo_weigths_path_default;
};

#endif // CONFIG_H
