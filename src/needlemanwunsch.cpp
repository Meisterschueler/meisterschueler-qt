#include "needlemanwunsch.h"

#include "util.h"

NeedlemanWunsch::NeedlemanWunsch()
{
}

NeedlemanWunsch::~NeedlemanWunsch() {
}

QByteArray NeedlemanWunsch::getAlignments(const QByteArray &seq1, const QByteArray &seq2, const char &offset) {
    int width = 30;

    int big = 10000;

    int n1 = seq1.length();
    int n2 = seq2.length();
    int *dist = new int[((n1+1)*(n2+1))];

    // Initialize Matrix
    for ( int i = 0; i < (n1+1)*(n2+1); i++)
        dist[ i ] = big;

    // Initialize Matrix bounds
    dist[ 0 ] = 0;

    for( int i = 1; i <= n1; ++i )
        dist[ i*(n2+1) ] = dist[ (i-1)*(n2+1) ] + cost_del;

    for( int j = 1; j <= n2; ++j )
        dist[ j ] = dist[ j-1 ] + cost_ins;

    // Calculate Matrix
    for( int i = 1; i <= n1; ++i ) {
        for( int j = MAX2(1, i-width); j <= MIN2(n2, i+width); ++j ) {
            //for( int j = 1; j <= n2; ++j ) {
            int dist_del = dist[ (i-1)*(n2+1) + (j  ) ] + cost_del;
            int dist_ins = dist[ (i  )*(n2+1) + (j-1) ] + cost_ins;
            //int dist_sub = dist[ (i-1)*(n2+1) + (j-1) ] + ( seq1.charAt(i-1) == seq2.charAt(j-1) ? cost_mat : cost_sub );
            int dist_sub = dist[ (i-1)*(n2+1) + (j-1) ] + getSimilarity(seq1.at(i-1)+offset, seq2.at(j-1));
            dist[ i*(n2+1) + j ] = MIN3( dist_del, dist_ins, dist_sub );
        }
    }

    // Back trace
    QByteArray align = "";
    int i = n1;
    int j = n2;

    // For DTW only
    if (i > j) {
        while ((i > 0) && (dist[ i*(n2+1) + j ] == big )) {
            align.insert(0, DELETED);
            i--;
        }
    } else {
        while ((j > 0) && (dist[ i*(n2+1) + j ]  == big )) {
            align.insert(0, INSERT);
            j--;
        }
    }

    int up;
    int diag;
    int left;
    while (i>0 || j>0) {
        if ( i > 0 )
            up = dist[ i*(n2+1) + j] - dist[ (i-1)*(n2+1) + (j  )];
        else
            up = -1;

        if ( j > 0 )
            left = dist[ i*(n2+1) + j] - dist[ (i  )*(n2+1) + (j-1)];
        else
            left = -1;

        if ( i > 0 && j > 0 )
            diag = dist[ i*(n2+1) + j] - dist[ (i-1)*(n2+1) + (j-1)];
        else
            diag = -1;

        if (diag>=up && diag>=left) {
            if (diag == cost_mat) {
                align.insert(0, MATCH);
                i--;
                j--;
            } else {
                align.insert(0, WRONG);
                i--;
                j--;
            }
        } else if (up>=left) {
            align.insert(0, DELETED);
            i--;
        } else {
            align.insert(0, INSERT);
            j--;
        }
    }
    delete[] dist;
    return align;
}

int NeedlemanWunsch::getSimilarity(const unsigned char i, const unsigned char j) {
    if (i == j) {
        return cost_mat;
    } else if (i-j == 1 || j-i == 1) {
        return 10;
    } else {
        return 200;
    }
}
