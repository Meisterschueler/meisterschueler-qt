#ifndef SCORE_H
#define SCORE_H

#include "Fraction.h"

class NoteOnEvent;
class NoteOffEvent;

enum Finger { UNKNOWN_FINGER, THUMB, POINTER, MIDDLE, RING, LITTLE };
enum Hand { UNKNOWN_HAND, LEFT, RIGHT, BOTH };
enum Status { UNKNOWN_STATUS, PLAYED, MISSED, EXTRA, FAILED, OPEN };

class Score
{
public:
    Score();

    char pitch;
    Fraction duration;
    Fraction position;

    NoteOnEvent *noteOn;
    NoteOffEvent *noteOff;
    Finger finger;
    Hand hand;
    Status status;
};

#endif // SCORE_H
