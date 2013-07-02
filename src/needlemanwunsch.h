#ifndef NEEDLEMANWUNSCH_H
#define NEEDLEMANWUNSCH_H

#include <QByteArray>

class NeedlemanWunsch
{
public:
    NeedlemanWunsch();
    ~NeedlemanWunsch();

    static QByteArray getAlignments(const QByteArray& seq1, const QByteArray& seq2, const char& offset=0);

    static constexpr char MATCH = 'm';
    static constexpr char DELETED = 'd';
    static constexpr char INSERT = 'i';
    static constexpr char WRONG = 'w';
    static constexpr char OPEN = 'o';

private:
    static constexpr char cost_mat = 0;
    static constexpr char cost_del = 10;
    static constexpr char cost_ins = 10;
    static constexpr char cost_wro = 10;

    const short matrix_size = 256;

    static int getSimilarity(const unsigned char i, const unsigned char j);
};

#endif // NEEDLEMANWUNSCH_H
