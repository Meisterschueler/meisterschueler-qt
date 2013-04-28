#ifndef SCORE_H
#define SCORE_H

#include <QSharedPointer>

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

    QSharedPointer<NoteOnEvent> noteOn;
    QSharedPointer<NoteOffEvent> noteOff;
    Finger finger;
    Hand hand;
    Status status;

    bool operator<(const Score& rhs) const {
        if (this->position < rhs.position) {
            return true;
        } else if (this->position == rhs.position && this->pitch < rhs.pitch){
            return true;
        } else {
            return false;
        }
    }
};


#endif // SCORE_H
