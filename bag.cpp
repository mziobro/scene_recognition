#include "bag.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

Bag::Bag(QString name)
    : m_name(name)
{

}

Bag::Bag(Bag &other)
{
    this->m_bagOfwords = other.m_bagOfwords;
    this->m_name = other.m_name;
}

int Bag::getOccurance(int name) const
{
    for(auto &word : m_bagOfwords)
    {
        if(word.idx == name)
        {
            return word.occurances;
        }
    }

    return 0;
}

void Bag::addWord(int idx)
{
    bool alreadyPresent = false;
    for(auto &word : m_bagOfwords)
    {
        if(word.idx == idx)
        {
            word.occurances++;
            alreadyPresent = true;
            break;
        }
    }
    if(!alreadyPresent)
        m_bagOfwords.push_back({idx, 1, 0});
}

void Bag::addWord(int idx, int occcurances)
{
    m_bagOfwords.push_back({idx, occcurances, 0});
}

void Bag::addWeightedWord(int idx, float weight)
{
    m_bagOfwords.push_back({idx, 0, weight});
}

void Bag::set_weight(float weight, int idx)
{
    for (auto& word : m_bagOfwords)
    {
        if (word.idx == idx)
            word.weight = weight;
    }
}

void Bag::load_bags()
{
    QDir dir;
    QFile file;
    for (auto file : dir.entryList(QDir::Files))
    {
        if (file.contains(".json"))
        {
            QJsonDocument json_doc;

        }
    }
}

void Bag::clear()
{
    m_bagOfwords.clear();
}

//!
//! \brief Updates Bag with reference Bag. This should be used
//!  only in calibration
//! \param other - Reference bag recognized during calibratoin
//!
void Bag::merge(Bag other)
{
    bool isAdded = false;

    //Iterate through every recognized object
    for (auto obj : other.m_bagOfwords)
    {
        //Iterate through every existing object
        for (auto &word : this->m_bagOfwords)
        {
            //Check, if bag already contains current object
            if(obj.idx == word.idx)
            {
                //Object already exists in destnation bag
                isAdded = true;
                //Check number of current class objects detected on image
                //If there is more objects than already remebered,
                //set number of occcurances to higher
                if(obj.occurances > word.occurances)
                    word.occurances = obj.occurances;
            }
        }

        //If object was not previously added to Bag, add it
        if(!isAdded)
            this->addWord(obj.idx);
        else
            isAdded = false;
    }
}

void Bag::save()
{
    QDir calib_files_dir{m_path};
    QString output_path = m_path + m_name + ".json";
    QFile file(output_path);

    if (!calib_files_dir.exists())
        calib_files_dir.mkpath(m_path);

    if(!file.open(QIODevice::Text | QIODevice::ReadWrite | QIODevice::Truncate))
    {
        qDebug() << "Cannot open file " + output_path;
        return;
    }
    QJsonObject obj;


    for (auto word : m_bagOfwords){
        obj[QString::number(word.idx)] = double(word.weight);
    }

    QJsonDocument doc;
    doc.setObject(obj);
    file.write(doc.toJson());

    file.close();

    qDebug() << "File " + output_path + " created";
}

QString Bag::getName() const
{
    return m_name;
}

Bag &Bag::operator=(const Bag &other)
{
    this->m_bagOfwords = other.m_bagOfwords;
    this->m_name = other.m_name;

    return *this;
}
