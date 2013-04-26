#ifndef NEEDLEMANWUNSCH_H
#define NEEDLEMANWUNSCH_H

#include <QString>

class NeedlemanWunsch
{
public:
    NeedlemanWunsch();
    ~NeedlemanWunsch();

    QString getAlignments(QString seq1, QString seq2);

private:
    const char MATCH = 'm';
    const char DELETED = 'd';
    const char INSERT = 'i';
    const char WRONG = 'w';
    const char OPEN = 'o';

    const char cost_mat = 0;
    const char cost_del = 10;
    const char cost_ins = 10;
    const char cost_wro = 10;

    const short matrix_size = 256;

    int *similarity;

    int getSimilarity(int i, int j);
    void calcSimilarityMatrix();
};

#endif // NEEDLEMANWUNSCH_H
