#include "hanonsongfactory.h"

#include "guidoservice.h"
#include "scoreservice.h"
#include "song.h"
#include "score.h"

HanonSongFactory::HanonSongFactory()
{
}

QList<Song> HanonSongFactory::getSongs() const {
    QList<Song> songs;
    for (int i = 0; i < 36; ++i) {
        songs.append(getNo(i));
    }

    return songs;
}

Song HanonSongFactory::getNo(int no) const {
    QList<Score> leftScores;
    QList<Score> rightScores;
    QString name = "";
    QString description = "";
    unsigned long long int id = 0L;
    QString meter = "2/4";

    QList<Sequence> leftSequences;
    QList<Sequence> rightSequences;

    QList<Sequence2> bothSequences;
    switch (no) {
    case 0: {
        name = "1";
        description = "";
        id = 100L;

        QString patternUp = "[c0/16 e f g a g f e]";
        QVector<int> leftFingersUp { 5, 4, 3, 2, 1, 2, 3, 4 };
        QVector<int> rightFingersUp { 1, 2, 3, 4, 5, 4, 3, 2 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 e d c b1 c2 d e]";
        QVector<int> leftFingersDown { 1, 2, 3, 4, 5, 4, 3, 2 };
        QVector<int> rightFingersDown { 5, 4, 3, 2, 1, 2, 3, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 1: {
        name = "2";
        description = "";
        id = 200L;

        QString patternUp = "[c0/16 e a g f g f e]";
        QVector<int> leftFingersUp { 5, 3, 1, 2, 3, 2, 3, 4 };
        QVector<int> rightFingersUp { 1, 2, 5, 4, 3, 4, 3, 2 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 d b1 c2 d c d e]";
        QVector<int> leftFingersDown { 1, 3, 5, 4, 3, 4, 3, 2 };
        QVector<int> rightFingersDown { 5, 2, 1, 2, 3, 2, 3, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 2: {
        name = "3";
        description = "";
        id = 300L;

        QString patternUp = "[c0/16 e a g f e f g]";
        QVector<int> leftFingersUp { 5, 3, 1, 2, 3, 4, 3, 2 };
        QVector<int> rightFingersUp { 1, 2, 5, 4, 3, 2, 3, 4 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 d b1 c2 d e d c]";
        QVector<int> leftFingersDown { 1, 3, 5, 4, 3, 2, 3, 4 };
        QVector<int> rightFingersDown { 5, 2, 1, 2, 3, 4, 3, 2 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 3: {
        name = "4";
        description = "Special exercise for the 3rd, 4th and 5th fingers of the hand.";
        id = 400L;

        QString patternUp = "[c0/16 d c e a g f e]";
        QVector<int> leftFingersUp { 5, 4, 5, 3, 1, 2, 3, 4 };
        QVector<int> rightFingersUp { 1, 2, 1, 2, 5, 4, 3, 2 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 f g d b1 c2 d e]";
        QVector<int> leftFingersDown { 1, 2, 1, 3, 5, 4, 3, 2 };
        QVector<int> rightFingersDown { 5, 4, 5, 2, 1, 2, 3, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 4: {
        name = "5";
        description = "We repeat, that the fingers should be lifted high, and with precision, until this entire volm is mastered.";
        id = 500L;

        QString patternUp = "[c0/16 a g a f g e f]";
        QVector<int> leftFingersUp { 5, 1, 2, 1, 3, 2, 4, 3 };
        QVector<int> rightFingersUp { 1, 5, 4, 5, 3, 4, 2, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[c2/16 d c e d f e g]";
        QVector<int> leftFingersDown { 5, 4, 5, 3, 4, 2, 3, 1 };
        QVector<int> rightFingersDown { 1, 2, 1, 3, 2, 4, 3, 5 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 5: {
        name = "6";
        description = "To obtain the good results which we promise those who study this work, it is indispensable to play daily, at least once, the exercise already learned.";
        id = 600L;

        QString patternUp1 = "[c0/16 a g a f a e a]";
        QString patternUp2 = "[c0/16 a g a f a e d]";
        QVector<int> leftFingersUp1 { 5, 1, 2, 1, 3, 1, 4, 1 };
        QVector<int> leftFingersUp2 { 5, 1, 2, 1, 3, 1, 4, 5 };
        QVector<int> rightFingersUp1 { 1, 5, 4, 5, 3, 5, 2, 5 };
        QVector<int> rightFingersUp2 { 1, 5, 4, 5, 3, 5, 2, 1 };
        QVector<int> stepsUp1  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        QVector<int> stepsUp2 {13};
        bothSequences << Sequence2(patternUp1, leftFingersUp1, rightFingersUp1, stepsUp1);
        bothSequences << Sequence2(patternUp2, leftFingersUp2, rightFingersUp2, stepsUp2);

        QString patternDown1 = "[g2/16 b1 c2 b1 d2 b1 e2 b1]";
        QString patternDown2 = "[g2/16 b1 c2 b1 d2 b1 e2 d2]";
        QVector<int> leftFingersDown1 { 1, 5, 4, 5, 3, 5, 2, 5 };
        QVector<int> leftFingersDown2 { 1, 5, 4, 5, 3, 5, 2, 3 };
        QVector<int> rightFingersDown1 { 5, 1, 2, 1, 3, 1, 4, 1 };
        QVector<int> rightFingersDown2 { 5, 1, 2, 1, 3, 1, 4, 3 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        QVector<int> stepsDown2 {-13};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }

    case 6: {
        name = "7";
        description = "Exercise of the greatest importance for the 3rd, 4th and 5th fingers.";
        id = 700L;

        QString patternUp = "[c0/16 e d f e g f e]";
        QVector<int> leftFingersUp { 5, 3, 4, 2, 3, 1, 2, 3 };
        QVector<int> rightFingersUp { 1, 3, 2, 4, 3, 5, 4, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 e f d e c d e]";
        QVector<int> leftFingersDown { 1, 3, 2, 4, 3, 5, 4, 3 };
        QVector<int> rightFingersDown { 5, 3, 4, 2, 3, 1, 3, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 7: {
        name = "8";
        description = "Very important exercise for all five fingers.";
        id = 800L;

        QString patternUp = "[c0/16 e g a f g e f]";
        QVector<int> leftFingersUp { 5, 4, 2, 1, 3, 2, 4, 3 };
        QVector<int> rightFingersUp { 1, 2, 4, 5, 3, 4, 2, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 e c b1 d2 c e d]";
        QVector<int> leftFingersDown { 1, 2, 4, 5, 3, 4, 2, 3 };
        QVector<int> rightFingersDown { 5, 4, 2, 1, 3, 2, 4, 3};
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 8: {
        name = "9";
        description = "Extension of the 4th and 5th, and general finger-exercise.";
        id = 900L;

        QString patternUp = "[c0/16 e f e g f a g]";
        QVector<int> leftFingersUp { 5, 4, 3, 4, 2, 3, 1, 2 };
        QVector<int> rightFingersUp { 1, 2, 3, 2, 4, 3, 5, 4 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown1 = "[g2/16 e d e c d b1 c2]";
        QString patternDown2 = "[g2/16 e d e c d c d]";
        QVector<int> leftFingersDown1 { 1, 2, 3, 2, 4, 3, 5, 4 };
        QVector<int> leftFingersDown2 { 1, 2, 3, 2, 4, 3, 4, 3 };
        QVector<int> rightFingersDown1 { 5, 4, 3, 4, 2, 3, 1, 2 };
        QVector<int> rightFingersDown2 { 5, 4, 3, 4, 2, 3, 2, 3 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        QVector<int> stepsDown2 {-13};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }

    case 9: {
        name = "10";
        description = "Preparation for the trill, for the 3rd and 4th fingers of the left hand in ascending (1); and for the 3rd and 4th of the right, descending (2).";
        id = 1000L;

        QString patternUp = "[c0/16 a g f e f e f]";
        QVector<int> leftFingersUp { 5, 1, 2, 3, 4, 3, 4, 3 };
        QVector<int> rightFingersUp { 1, 5, 4, 3, 2, 3, 2, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 b1 c2 d e d e d]";
        QVector<int> leftFingersDown { 1, 5, 4, 3, 2, 3, 2, 3 };
        QVector<int> rightFingersDown { 5, 1, 2, 3, 4, 3, 4, 3 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 10: {
        name = "11";
        description = "Another preparation for the trill, for the 4th and 5th fingers.";
        id = 1100L;

        QString patternUp = "[c0/16 e a g a g f g]";
        QVector<int> leftFingersUp { 5, 3, 1, 2, 1, 2, 3, 2 };
        QVector<int> rightFingersUp { 1, 2, 5, 4, 5, 4, 3, 4 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 d b1 c2 b1 c2 d c]";
        QVector<int> leftFingersDown { 1, 3, 5, 4, 5, 4, 3, 2 };
        QVector<int> rightFingersDown { 5, 2, 1, 2, 1, 2, 3, 2 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 11: {
        name = "12";
        description = "Extension of 1-5, and exercise for 3-4-5.";
        id = 1200L;

        QString patternUp1 = "[g0/16 c e d c d e c]";
        QString patternUp2 = "[a0/16 c e d c d e c]";
        QString patternUp3 = "[a0/16 c e d c d e f]";
        QVector<int> leftFingersUp12 { 1, 5, 3, 4, 5, 4, 3, 5 };
        QVector<int> leftFingersUp3 { 1, 5, 3, 4, 5, 4, 3, 2 };
        QVector<int> rightFingersUp12 { 5, 1, 3, 2, 1, 2, 3, 1 };
        QVector<int> rightFingersUp3 { 5, 1, 3, 2, 1, 2, 3, 4 };
        QVector<int> stepsUp1  {0};
        QVector<int> stepsUp2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        QVector<int> stepsUp3 {13};
        bothSequences << Sequence2(patternUp1, leftFingersUp12, rightFingersUp12, stepsUp1);
        bothSequences << Sequence2(patternUp2, leftFingersUp12, rightFingersUp12, stepsUp2);
        bothSequences << Sequence2(patternUp3, leftFingersUp3, rightFingersUp3, stepsUp3);

        QString patternDown1 = "[c2/16 a f g a g f a]";
        QString patternDown2 = "[c0/16 g e f g f e f]";
        QVector<int> leftFingersDown12 { 1, 5, 3, 4, 5, 4, 3, 5 };
        QVector<int> rightFingersDown12 { 5, 1, 3, 2, 1, 2, 3, 4 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        QVector<int> stepsDown2 {0};
        bothSequences << Sequence2(patternDown1, leftFingersDown12, rightFingersDown12, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown12, rightFingersDown12, stepsDown2);

        break;
    }

    case 12: {
        name = "13";
        description = "(3-4-5)";
        id = 1300L;

        QString patternUp = "[e0/16 c f d g e f g]";
        QVector<int> leftFingersUp { 3, 5, 2, 4, 1, 3, 2, 1 };
        QVector<int> rightFingersUp { 3, 1, 4, 2, 5, 3, 4, 5 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[e2/16 g d f e c d e]";
        QVector<int> leftFingersDown { 3, 1, 4, 2, 3, 5, 4, 3 };
        QVector<int> rightFingersDown { 3, 5, 2, 4, 3, 1, 3, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 13: {
        name = "14";
        description = "(3-4) Another preparation for the trill, for the 3rd and 4th fingers.";
        id = 1400L;

        QString patternUp = "[c0/16 d f e f e g f]";
        QVector<int> leftFingersUp { 5, 4, 2, 3, 2, 3, 1, 2 };
        QVector<int> rightFingersUp { 1, 2, 4, 3, 4, 3, 5, 4 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 f d e d e c d]";
        QVector<int> leftFingersDown { 1, 2, 4, 3, 4, 3, 5, 4 };
        QVector<int> rightFingersDown { 5, 4, 2, 3, 2, 3, 1, 3 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 14: {
        name = "15";
        description = "Extension of 1-2, and exercise for all 5 fingers.";
        id = 1500L;

        QString patternUp1 = "[c0/16 e d f e g f a]";
        QString patternUp2 = "[c0/16 e d f e g f g]";
        QVector<int> leftFingersUp1 { 5, 3, 4, 2, 3, 1, 2, 1 };
        QVector<int> leftFingersUp2 { 5, 3, 4, 2, 3, 1, 3, 2 };
        QVector<int> rightFingersUp1 { 1, 2, 1, 3, 2, 4, 3, 5 };
        QVector<int> rightFingersUp2 { 1, 2, 1, 3, 2, 4, 3, 4 };
        QVector<int> stepsUp1 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        QVector<int> stepsUp2 {13};
        bothSequences << Sequence2(patternUp1, leftFingersUp1, rightFingersUp1, stepsUp1);
        bothSequences << Sequence2(patternUp2, leftFingersUp2, rightFingersUp2, stepsUp2);

        QString patternDown1 = "[g2/16 e f d e c d b1]";
        QString patternDown2 = "[g2/16 e f d e c d c]";
        QVector<int> leftFingersDown1 { 1, 2, 1, 3, 2, 4, 3, 5 };
        QVector<int> leftFingersDown2 { 1, 2, 1, 3, 2, 4, 3, 4 };
        QVector<int> rightFingersDown1 { 5, 3, 4, 2, 3, 1, 2, 1 };
        QVector<int> rightFingersDown2 { 5, 3, 4, 2, 3, 1, 3, 2 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10,- 11, -12};
        QVector<int> stepsDown2 {-13};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }

    case 15: {
        name = "16";
        description = "Extension of 3-5, and exercise for 3-4-5.";
        id = 1600L;

        QString patternUp = "[c0/16 e0 d0 e0 a0 g0 f0 g0]";
        QVector<int> leftFingersUp { 5, 3, 4, 3, 1, 2, 3, 2 };
        QVector<int> rightFingersUp { 1, 3, 2, 3, 5, 4, 3, 4 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 d2 e2 d2 b1 c2 d2 c2]";
        QVector<int> leftFingersDown { 1, 3, 2, 3, 5, 4, 3, 4 };
        QVector<int> rightFingersDown { 1, 3, 2, 3, 5, 4, 3, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 16: {
        name = "17";
        description = "Extension of 1-2, 2-4, 4-5, and exercise for 3-4-5.";
        id = 1700L;

        QString patternUp1 = "[c0/16 e a g b a g a]";
        QString patternUp2 = "[c0/16 e a g b a g f]";
        QVector<int> leftFingersUp1 { 5, 4, 2, 3, 1, 2, 3, 2 };
        QVector<int> leftFingersUp2 { 5, 4, 2, 3, 1, 2, 3, 4 };
        QVector<int> rightFingersUp1 { 1, 2, 4, 3, 5, 4, 3, 4 };
        QVector<int> rightFingersUp2 { 1, 2, 4, 3, 5, 4, 3, 2 };
        QVector<int> stepsUp1  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        QVector<int> stepsUp2 {13};
        bothSequences << Sequence2(patternUp1, leftFingersUp1, rightFingersUp1, stepsUp1);
        bothSequences << Sequence2(patternUp2, leftFingersUp2, rightFingersUp2, stepsUp2);

        QString patternDown1 = "[g2/16 d b1 c2 a1 b c2 a1]";
        QString patternDown2 = "[g2/16 d b1 c2 a1 b c2 b1]";
        QVector<int> leftFingersDown1 { 1, 2, 4, 3, 5, 4, 3, 5 };
        QVector<int> leftFingersDown2 { 1, 2, 4, 3, 5, 4, 3, 5 };
        QVector<int> rightFingersDown1 { 5, 3, 2, 3, 1, 2, 3, 1 };
        QVector<int> rightFingersDown2 { 5, 3, 2, 3, 1, 2, 3, 1 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11};
        QVector<int> stepsDown2 {-12};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }

    case 17: {
        name = "18";
        description = "(1-2-3-4-5)";
        id = 1800L;

        QString patternUp = "[c0/16 d f e g f d e]";
        QVector<int> leftFingersUp { 5, 4, 2, 3, 1, 2, 4, 3 };
        QVector<int> rightFingersUp { 1, 2, 4, 3, 5, 4, 2, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 f d e c d f e]";
        QVector<int> leftFingersDown { 1, 2, 4, 3, 5, 4, 2, 3 };
        QVector<int> rightFingersDown { 5, 4, 2, 3, 1, 2, 4, 3 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 18: {
        name = "19";
        description = "(1-2-3-4-5)";
        id = 1900L;

        QString patternUp = "[c0/16 a f g a f e g]";
        QVector<int> leftFingersUp { 5, 1, 3, 2, 1, 3, 4, 2 };
        QVector<int> rightFingersUp { 1, 5, 3, 4, 5, 3, 2, 4 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 b1 d2 c b1 d2 e c]";
        QVector<int> leftFingersDown { 1, 5, 3, 4, 5, 3, 2, 4 };
        QVector<int> rightFingersDown { 5, 1, 3, 2, 1, 3, 4, 2 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 19: {
        name = "20";
        description = "Extension of 2-4, 4-5, and exercise for 2-3-4.";
        id = 2000L;

        QString patternUp1 = "[e0/16 g c1 e c b0 c1 a0]";
        QString patternUp2 = "[e0/16 g c1 e c b0 c1 g0]";
        QVector<int> leftFingersUp12 { 5, 4, 2, 1, 2, 3, 2, 4 };
        QVector<int> rightFingersUp12 { 1, 2, 4, 5, 4, 3, 4, 2 };
        QVector<int> stepsUp1  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        QVector<int> stepsUp2 {14};
        bothSequences << Sequence2(patternUp1, leftFingersUp12, rightFingersUp12, stepsUp1);
        bothSequences << Sequence2(patternUp2, leftFingersUp12, rightFingersUp12, stepsUp2);

        QString patternDown1 = "[e3/16 c3 g2 e2 g2 f2 g2 e2]";
        QString patternDown2 = "[e3/16 c3 g2 e2 g2 f2 g2 f2]";
        QVector<int> leftFingersDown1 { 1, 2, 4, 5, 3, 4, 3, 5 };
        QVector<int> leftFingersDown2 { 1, 2, 4, 5, 3, 4, 3, 4 };
        QVector<int> rightFingersDown1 { 5, 4, 2, 1, 3, 2, 3, 1 };
        QVector<int> rightFingersDown2 { 5, 4, 2, 1, 3, 2, 3, 2 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        QVector<int> stepsDown2 {-14};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }
    case 20: {
        name = "21";
        id = 2100L;
        meter = "4/4";

        QString patternUp = "[c0/16 d e d c e f g a g f g a g f e]";
        QVector<int> leftFingersUp { 5, 4, 3, 4, 5, 4, 3, 2, 1, 2, 3, 2, 1, 2, 3, 4 };
        QVector<int> rightFingersUp { 1, 2, 3, 2, 1, 2, 3, 4, 5, 4, 3, 4, 5, 4, 3, 2 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown1 = "[g2/16 f e f g e d c b1 c2 d c b1 c2 d e]";
        QString patternDown2 = "[a0/16 g f g a f e d c d e d c d e d]";
        QVector<int> leftFingersDown1 { 1, 2, 3, 2, 1, 2, 3, 4, 5, 4, 3, 4, 5, 4, 3, 2 };
        QVector<int> leftFingersDown2 { 1, 2, 3, 2, 1, 2, 3, 4, 5, 4, 3, 4, 5, 4, 3, 4 };
        QVector<int> rightFingersDown1 { 5, 4, 3, 4, 5, 4, 3, 2, 1, 2, 3, 2, 1, 2, 3, 4 };
        QVector<int> rightFingersDown2 { 5, 4, 3, 4, 5, 4, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        QVector<int> stepsDown2 {0};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }
    case 21: {
        name = "22";
        id = 2200L;
        meter = "C";

        QString patternUp = "[c0/16 e d e c e f g a f g f a g f e]";
        QVector<int> leftFingersUp { 5, 3, 2, 3, 5, 4, 3, 2, 1, 3, 2, 3, 1, 2, 3, 4 };
        QVector<int> rightFingersUp { 1, 3, 2, 3, 1, 2, 3, 4, 5, 3, 4, 3, 5, 4, 3, 2 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown1 = "[g2/16 e f e g e d c b1 d2 c d b1 c2 d e]";
        QString patternDown2 = "[a0/16 f g f a f e d c e d e c f e d]";
        QVector<int> leftFingersDown1 { 1, 3, 2, 3, 1, 2, 3, 4, 5, 3, 4, 3, 5, 4, 3, 2 };
        QVector<int> leftFingersDown2 { 1, 3, 2, 3, 1, 2, 3, 4, 5, 3, 4, 3, 5, 2, 3, 4 };
        QVector<int> rightFingersDown1 { 5, 3, 4, 3, 5, 4, 3, 2, 1, 3, 2, 3, 1, 2, 3, 4 };
        QVector<int> rightFingersDown2 { 5, 3, 4, 3, 5, 4, 3, 2, 1, 3, 2, 3, 1, 4, 3, 2 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        QVector<int> stepsDown2 {0};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }
    case 22: {
        name = "23";
        id = 2300L;
        meter = "C";

        QString patternUp = "[c0 d e d c d e d c a g f e f g f]";
        QVector<int> leftFingersUp { 5, 4, 3, 4, 5, 4, 3, 4, 5, 1, 2, 3, 4, 3, 2, 3 };
        QVector<int> rightFingersUp { 1, 2, 3, 2, 1, 2, 3, 2, 1, 5, 4, 3, 2, 3, 4, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2 f e f g f e f g b1 c2 d e d c d]";
        QVector<int> leftFingersDown { 1, 2, 3, 2, 1, 2, 3, 2, 1, 5, 4, 3, 2, 3, 4, 3 };
        QVector<int> rightFingersDown { 5, 4, 3, 4, 5, 4, 3, 4, 5, 1, 2, 3, 4, 3, 2, 3 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }
    case 23: {
        name = "24";
        id = 2400L;
        meter = "C";

        QString patternUp = "[e0/16 d e c e d e c e d e c a f g e]";
        QVector<int> leftFingersUp { 3, 4, 3, 5, 3, 4, 3, 5, 3, 4, 3, 5, 1, 3, 2, 4 };
        QVector<int> rightFingersUp { 3, 2, 3, 1, 3, 2, 3, 1, 3, 2, 3, 1, 5, 3, 4, 2 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[e2/16 g f g e g f g e g f g b1 d2 c e]";
        QVector<int> leftFingersDown { 3, 1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 5, 3, 4, 2 };
        QVector<int> rightFingersDown { 3, 5, 4, 5, 3, 5, 4, 5, 3, 5, 4, 5, 1, 3, 2, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }
    case 24: {
        name = "25";
        id = 2500L;
        meter = "C";

        QString patternUp12 = "[c0/16 d e c d e f d e f g f e g f e]";
        QVector<int> leftFingersUp1 { 5, 4, 3, 5, 4, 3, 2, 4, 3, 2, 1, 2, 3, 1, 3, 4 };
        QVector<int> leftFingersUp2 { 5, 4, 3, 5, 4, 3, 2, 4, 3, 2, 1, 2, 3, 1, 3, 4 };
        QVector<int> rightFingersUp1 { 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 4, 3, 5, 4, 3 };
        QVector<int> rightFingersUp2 { 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 4, 3, 5, 3, 2 };
        QVector<int> stepsUp1  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        QVector<int> stepsUp2 {13};
        bothSequences << Sequence2(patternUp12, leftFingersUp1, rightFingersUp1, stepsUp1);
        bothSequences << Sequence2(patternUp12, leftFingersUp2, rightFingersUp2, stepsUp2);

        QString patternDown12 = "[g2/16 f e g f e d f e d c e d c d e]";
        QVector<int> leftFingersDown1 { 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 4, 3 };
        QVector<int> leftFingersDown2 { 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 3, 2 };
        QVector<int> rightFingersDown12 { 5, 4, 3, 5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 3, 4 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        QVector<int> stepsDown2 {-13};
        bothSequences << Sequence2(patternDown12, leftFingersDown1, rightFingersDown12, stepsDown1);
        bothSequences << Sequence2(patternDown12, leftFingersDown2, rightFingersDown12, stepsDown2);

        break;
    }
    case 25: {
        name = "26";
        id = 2600L;
        meter = "C";

        QString patternUp = "[e0/16 f g e d e f d c d e c a g a g]";
        QVector<int> leftFingersUp { 3, 2, 1, 3, 4, 3, 2, 4, 5, 4, 3, 5, 1, 2, 1, 2 };
        QVector<int> rightFingersUp { 3, 4, 5, 3, 2, 3, 4, 2, 1, 2, 3, 1, 5, 4, 5, 4 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 e f g f d e f e c d e d c d c]";
        QVector<int> leftFingersDown { 1, 3, 2, 1, 2, 4, 3, 2, 3, 5, 4, 3, 4, 3, 4, 3 };
        QVector<int> rightFingersDown { 5, 3, 4, 5, 4, 2, 3, 4, 3, 1, 2, 3, 2, 1, 2, 1 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }
    case 26: {
        name = "27";
        id = 2700L;
        meter = "C";

        QString patternUp = "[e0/16 f d e c e f g a g a g a g f e]";
        QVector<int> leftFingersUp { 3, 2, 4, 2, 5, 4, 3, 2, 1, 2, 1, 2, 1, 2, 3, 4 };
        QVector<int> rightFingersUp { 3, 4, 2, 3, 1, 2, 3, 4, 5, 4, 5, 4, 5, 4, 3, 2 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 f g e f e d c b1 c2 b1 c2 b1 c2 d e]";
        QVector<int> leftFingersDown { 1, 2, 1, 3, 1, 2, 3, 4, 5, 4, 5, 4, 5, 4, 3, 2 };
        QVector<int> rightFingersDown { 5, 4, 5, 3, 5, 4, 3, 2, 1, 2, 1, 2, 1, 2, 3, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }
    case 27: {
        name = "28";
        id = 2800L;
        meter = "C";

        QString patternUp = "[c0/16 e d e c e d e c a g a f g e f]";
        QVector<int> leftFingersUp { 5, 3, 4, 3, 5, 3, 4, 3, 5, 1, 2, 1, 3, 2, 4, 3 };
        QVector<int> rightFingersUp { 1, 3, 2, 3, 1, 3, 2, 3, 1, 5, 4, 5, 3, 4, 2, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[g2/16 e f e g e f e g b1 c2 b1 d2 c e d]";
        QVector<int> leftFingersDown { 1, 3, 2, 3, 1, 3, 2, 3, 1, 5, 4, 5, 3, 4, 2, 3 };
        QVector<int> rightFingersDown { 5, 3, 4, 3, 5, 3, 4, 3, 5, 1, 2, 1, 3, 2, 4, 3 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }
    case 28: {
        name = "29";
        id = 2900L;
        meter = "C";

        QString patternUp12 = "[c0/16 d c e d e d f e f e g f g f g]";
        QVector<int> leftFingersUp1 { 5, 4, 5, 3, 4, 3, 4, 2, 3, 2, 3, 1, 2, 1, 2, 1 };
        QVector<int> leftFingersUp2 { 5, 4, 5, 3, 4, 3, 4, 2, 3, 2, 3, 1, 3, 2, 3, 2 };
        QVector<int> rightFingersUp1 { 1, 2, 1, 3, 2, 3, 2, 4, 3, 4, 3, 5, 4, 5, 4, 5 };
        QVector<int> rightFingersUp2 { 1, 2, 1, 3, 2, 3, 2, 4, 3, 4, 3, 5, 4, 5, 3, 4 };
        QVector<int> stepsUp1  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        QVector<int> stepsUp2 {13};
        bothSequences << Sequence2(patternUp12, leftFingersUp1, rightFingersUp1, stepsUp1);
        bothSequences << Sequence2(patternUp12, leftFingersUp2, rightFingersUp2, stepsUp2);

        QString patternDown1 = "[g2/16 f g e f e f d e d e c d e d e]";
        QString patternDown2 = "[a0/16 g a f g f g e f e f d e f e d]";
        QVector<int> leftFingersDown1 { 1, 2, 1, 3, 2, 3, 2, 4, 3, 4, 3, 5, 4, 3, 4, 3 };
        QVector<int> leftFingersDown2 { 1, 2, 1, 3, 2, 3, 2, 4, 3, 4, 3, 5, 3, 2, 3, 4 };
        QVector<int> rightFingersDown1 { 5, 4, 5, 3, 4, 3, 4, 2, 3, 2, 3, 1, 2, 3, 2, 3 };
        QVector<int> rightFingersDown2 { 5, 4, 5, 3, 4, 3, 4, 2, 3, 2, 3, 1, 3, 4, 3, 2 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        QVector<int> stepsDown2 {0};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }
    case 29: {
        name = "30";
        id = 3000L;
        meter = "C";

        QString patternUp = "[c0/16 d c d c d c e a g a g a g a f]";
        QVector<int> leftFingersUp { 5, 4, 5, 4, 5, 4, 5, 3, 1, 2, 1, 2, 1, 2, 1, 3 };
        QVector<int> rightFingersUp { 1, 2, 1, 2, 1, 2, 1, 3, 5, 4, 5, 4, 5, 4, 5, 3 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown1 = "[g2/16 f g f g f g e b1 c2 b1 c2 b1 c2 b1 d2]";
        QString patternDown2 = "[g2 f g f g f g e b1 c2 b1 c2 b1 c2 b1 c2]";
        QVector<int> leftFingersDown1 { 1, 2, 1, 2, 1, 2, 1, 3, 5, 4, 5, 4, 5, 4, 5, 3 };
        QVector<int> leftFingersDown2 { 1, 2, 1, 2, 1, 2, 1, 3, 5, 4, 5, 4, 5, 4, 5, 4 };
        QVector<int> rightFingersDown1 { 5, 4, 5, 4, 5, 4, 5, 3, 1, 2, 1, 2, 1, 2, 1, 3 };
        QVector<int> rightFingersDown2 { 5, 4, 5, 4, 5, 4, 5, 3, 1, 2, 1, 2, 1, 2, 1, 2 };
        QVector<int> stepsDown1 {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        QVector<int> stepsDown2 {-13};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }
    case 30: {
        name = "31";
        id = 3100L;
        meter = "3/4";

        QString leftPatternUp = "[c0/16 c1 b0 c1 a0 c1 g0 c1 f0 c1 e0 c1]";
        QString rightPatternUp = "[e1/16 e2 d e c e b1 e2 a1 e2 g1 e2]";
        QVector<int> leftFingersUp { 5, 1, 2, 1, 3, 1, 4, 1, 5, 1, 5, 1 };
        QVector<int> rightFingersUp { 1, 5, 4, 5, 3, 5, 2, 5, 1, 5, 1, 5 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        leftSequences << Sequence(leftPatternUp, leftFingersUp, stepsUp);
        rightSequences << Sequence(rightPatternUp, rightFingersUp, stepsUp);

        QString leftPatternDown = "[c3/16 c2 d c e c f c g c a c]";
        QString rightPatternDown = "[e4/16 e3 f e g e a e b e c4 e3]";
        QVector<int> leftFingersDown { 1, 5, 4, 5, 3, 5, 2, 5, 1, 5, 1, 5 };
        QVector<int> rightFingersDown { 5, 1, 2, 1, 3, 1, 4, 1, 5, 1, 5, 1 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        leftSequences << Sequence(leftPatternDown, leftFingersDown, stepsDown);
        rightSequences << Sequence(rightPatternDown, rightFingersDown, stepsDown);

        break;
    }
    case 31: {
        name = "38";
        id = 3800L;
        meter = "2/4";

        QString patternUp = "[c0/16 d e f g a b c1]";
        QVector<int> leftFingersUp { 5, 4, 3, 2, 1, 3, 2, 1 };
        QVector<int> rightFingersUp { 1, 2, 3, 1, 2, 3, 4, 5 };
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[c3/16 b2 a2 g2 f2 e2 d2 c2]";
        QVector<int> leftFingersDown { 1, 2, 3, 1, 2, 3, 4, 5 };
        QVector<int> rightFingersDown { 5, 4, 3, 2, 1, 3, 2, 1 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }
    case 32: {
        name = "50";
        id = 5000L;
        meter = "C";

        QString patternUp12 = "[{c0,e0} {d0,f0} {e0,g0} {d0,f0}]";
        QVector<int> leftFingersUp12 { 5, 3, 4, 2, 3, 1, 4, 2 };
        QVector<int> rightFingersUp12 { 1, 3, 2, 4, 3, 5, 2, 4 };
        QVector<int> stepsUp1  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        QVector<int> stepsUp2 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        bothSequences << Sequence2(patternUp12, leftFingersUp12, rightFingersUp12, stepsUp1);
        bothSequences << Sequence2(patternUp12, leftFingersUp12, rightFingersUp12, stepsUp2);

        QString patternDown = "[{e2,g2} {d2,f2} {c2,e2} {d2,f2}]";
        QVector<int> leftFingersDown { 3, 1, 4, 2, 5, 3, 4, 2 };
        QVector<int> rightFingersDown { 3, 5, 2, 4, 1, 3, 2, 4 };
        QVector<int> stepsDown {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;
    }

    case 33: {
        name = "50a";
        id = 5001L;
        meter = "C";

        QString patternUp = "[{c0,e} {d,f} {e,g}]";
        QVector<int> leftFingersUp { 5, 3, 4, 2, 3, 1 };
        QVector<int> rightFingersUp { 1, 3, 2, 4, 3, 5 };
        QVector<int> stepsUp {0, 3, 6, 9, 12, 15, 18, 21};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown1 = "[{d3,f} {c,e} {b2,d3}]";
        QString patternDown2 = "[{d0,f} {c,e} {d,f}]";
        QVector<int> leftFingersDown1 { 4, 2, 5, 3, 3, 1 };
        QVector<int> leftFingersDown2 { 4, 2, 5, 3, 4, 2 };
        QVector<int> rightFingersDown1 { 2, 4, 1, 3, 3, 5 };
        QVector<int> rightFingersDown2 { 2, 4, 1, 3, 2, 4 };
        QVector<int> stepsDown1 {0, -3, -6, -9, -12, -15, -18};
        QVector<int> stepsDown2 {0};
        bothSequences << Sequence2(patternDown1, leftFingersDown1, rightFingersDown1, stepsDown1);
        bothSequences << Sequence2(patternDown2, leftFingersDown2, rightFingersDown2, stepsDown2);

        break;
    }

    case 34: {
        name = "54";
        id = 5400L;
        meter = "C";

        QString patternUp = "[{c1,e} {d,f} {c,e} {d,f} {c,e} {d,f} {c,e} {d,f} {e,g} {d,f} {e,g} {d,f} {e,g} {d,f} {e,g} {d,f}]";
        QVector<int> leftFingersUp { 5, 3, 4, 2, 5, 3, 4, 2, 5, 3, 4, 2, 5, 3, 4, 2, 3, 1, 4, 2, 3, 1, 4, 2, 3, 1, 4, 2, 3, 1, 4, 2};
        QVector<int> rightFingersUp { 1, 3, 2, 4, 1, 3, 2, 4, 1, 3, 2, 4, 1, 3, 2, 4, 3, 5, 2, 4, 3, 5, 2, 4, 3, 5, 2, 4, 3, 5, 2, 4};
        QVector<int> stepsUp {0, 1, 2, 3, 4, 5, 6, 7, 8};
        bothSequences << Sequence2(patternUp, leftFingersUp, rightFingersUp, stepsUp);

        QString patternDown = "[{d2,f} {c,e} {d,f} {c,e} {d,f} {c,e} {d,f} {c,e} {b1,d2} {c,e} {b1,d2} {c,e} {b1,d2} {c,e} {b1,d2} {c,e}]";
        QVector<int> leftFingersDown { 3, 1, 4, 2, 3, 1, 4, 2, 3, 1, 4, 2, 3, 1, 4, 2, 5, 3, 4, 2, 5, 3, 4, 2, 5, 3, 4, 2, 5, 3, 4, 2};
        QVector<int> rightFingersDown { 3, 5, 2, 4, 3, 5, 2, 4, 3, 5, 2, 4, 3, 5, 2, 4, 1, 3, 2, 4, 1, 3, 2, 4, 1, 3, 2, 4, 1, 3, 2, 4};
        QVector<int> stepsDown { 0, -1, -2, -3, -4, -5, -6};
        bothSequences << Sequence2(patternDown, leftFingersDown, rightFingersDown, stepsDown);

        break;

    }

    case 35: {
        name = "0";
        description = "Scale";
        id = 0L;

        QString pattern = "[c0 d e f g a b c1 d e f g a b c2 b1 a g f e d c b0 a g f e d]";
        QVector<int> leftFingers { 5, 4, 3, 2, 1, 3, 2, 1, 4, 3, 2, 1, 3, 2, 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 1, 2, 3, 4, 5 };
        QVector<int> rightFingers { 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 1, 2, 3, 4, 5, 4, 3, 2, 1, 3, 2, 1, 4, 3, 2, 1, 3, 2, 1 };
        QVector<int> steps = {0};
        bothSequences << Sequence2(pattern, leftFingers, rightFingers, steps);

        break;
    }

    }

    if (!bothSequences.isEmpty()) {
        for (Sequence2 sequence : bothSequences) {
            leftSequences.append(Sequence(sequence.m_pattern, sequence.m_leftFingers, sequence.m_steps));
            rightSequences.append(Sequence(sequence.m_pattern, sequence.m_rightFingers, sequence.m_steps));
        }
    }

    leftScores = sequencesToScores(leftSequences);
    rightScores = sequencesToScores(rightSequences);

    int oneOctaveUp = 7;
    rightScores = ScoreService::transposeStep(rightScores, oneOctaveUp);

    Song song;
    song.id = id;
    song.name = "No. " + name;
    song.description = description;
    song.gmn = leftSequences.at(0).m_pattern;

    song.voices.insert(LEFT, leftScores);
    song.voices.insert(RIGHT, rightScores);
    song.voices.insert(BOTH, ScoreService::merge(leftScores, rightScores));

    return song;
}

QList<Score> HanonSongFactory::sequencesToScores(const QList<Sequence>& sequences) const {
    QList<QString> patterns;
    QList<QVector<int>> fingers;
    QList<QVector<int>> steps;

    for (Sequence sequence : sequences) {
        patterns.append(sequence.m_pattern);
        fingers.append(sequence.m_fingers);
        steps.append(sequence.m_steps);
    }

    QList<Score> result;

    for (int i = 0; i < patterns.size(); i++) {
        QList<Score> scores = GuidoService::gmnToScores(patterns.at(i));
        scores = ScoreService::addFingers(scores, fingers.at(i));
        scores = ScoreService::transposeSteps(scores, steps.at(i));
        result = ScoreService::concat(result, scores);
    }

    return result;
}

