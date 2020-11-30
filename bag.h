#ifndef BAGOFWORDS_H
#define BAGOFWORDS_H

#include <vector>
#include <QString>

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

    void set_weight(float weight, int idx);

    void clear();
    void merge(Bag&);
    void save();

    QString getName() const;
    Bag &operator=(const Bag& other);

    std::vector<word> m_bagOfwords;
private:
    QString m_name;

    const QString path = "C:/Users/user/Documents/Master_App/";
};

#endif // BAGOFWORDS_H
