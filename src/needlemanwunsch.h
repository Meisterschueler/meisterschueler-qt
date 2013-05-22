#ifndef NEEDLEMANWUNSCH_H
#define NEEDLEMANWUNSCH_H

#include <QByteArray>

class NeedlemanWunsch
{
public:
    NeedlemanWunsch();
    ~NeedlemanWunsch();

    QByteArray getAlignments(QByteArray seq1, QByteArray seq2);

    static constexpr char MATCH = 'm';
    static constexpr char DELETED = 'd';
    static constexpr char INSERT = 'i';
    static constexpr char WRONG = 'w';
    static constexpr char OPEN = 'o';

private:
    const char cost_mat = 0;
    const char cost_del = 10;
    const char cost_ins = 10;
    const char cost_wro = 10;

    const short matrix_size = 256;

    int *similarity;

    int getSimilarity(const unsigned char i, const unsigned char j);
    void calcSimilarityMatrix();
};

#endif // NEEDLEMANWUNSCH_H
