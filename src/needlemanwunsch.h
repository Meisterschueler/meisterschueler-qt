#ifndef NEEDLEMANWUNSCH_H
#define NEEDLEMANWUNSCH_H

#include <QByteArray>

class NeedlemanWunsch
{
public:
    NeedlemanWunsch();
    ~NeedlemanWunsch();

    QByteArray getAlignments(const QByteArray& seq1, const QByteArray& seq2, const char& offset=0) const;

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

    int getSimilarity(const unsigned char i, const unsigned char j) const;
    void calcSimilarityMatrix();
};

#endif // NEEDLEMANWUNSCH_H
