#ifndef BAGOFWORDS_H
#define BAGOFWORDS_H

#include <vector>
#include <QString>

#include <QDir>

/*!
 * \brief The Bag class
 * \details klasa Bag jest odpowiedzialna za tworznie i przechowywanie wartości
 * BOVW. Właściwe wartości przechowywane są w wektorze m_bagOfWords.
 * Struktura word zawiera 3 pola: idx, czyli index klasy obiektu (wzięty z modelu sieci);
 * occurances - ile razy dany przedmiot pojawił się w danym pomieszczeniu, oraz weight (0-1)
 * - znormalizowana waga danego obiektu.
 *
 * Przy kalibracji zapisywana jest liczba wystąpień, czyli occurances. Wagi są liczone względem
 * wszystkich pomieszczeń.
 *
 * Nazwa pomieszczenia przechowywana jest w ziennej m_name
 *
 *
 */
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
