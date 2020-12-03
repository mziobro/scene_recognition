#ifndef BAGOFWORDS_H
#define BAGOFWORDS_H

#include <vector>
#include <QString>

#include <QDir>

class Bag
{
public:
    typedef struct {
        int idx;
        int occurances;
        float weight;
    } word;

    Bag(QString name);
    Bag(Bag& other);

    int getOccurance(int name) const;
    void addWord(int idx);
    void addWord(int idx, int occcurances);
    void addWeightedWord(int idx, float weight);

    void set_weight(float weight, int idx);

    void load_bags();
    void clear();
    void merge(Bag);
    void save();

    QString getName() const;
    Bag &operator=(const Bag& other);

    std::vector<word> m_bagOfwords;
private:
    QString m_name;

    const QString m_path = QDir::currentPath() +  "/Calib/";
};

#endif // BAGOFWORDS_H
