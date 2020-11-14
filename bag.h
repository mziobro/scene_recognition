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
    } word;

    Bag(QString name);

    int getOccurance(int name) const;
    void addWord(int idx);
    void clear();
    void merge(Bag&);
    void save();

    QString getName() const;

    std::vector<word> m_bagOfwords;
private:
    QString m_name;

    const QString path = "C:/Users/user/Documents/Master_App";
};

#endif // BAGOFWORDS_H
