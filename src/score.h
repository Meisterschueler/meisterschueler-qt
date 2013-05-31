#ifndef SCORE_H
#define SCORE_H

#include <QSharedPointer>

#include "Fraction.h"

#include "events.h"

class MidiPair;

enum Finger { UNKNOWN_FINGER = 0x01, THUMB = 0x02, POINTER = 0x04, MIDDLE = 0x08, RING = 0x10, LITTLE = 0x20 };
enum Hand { UNKNOWN_HAND = 0x01, LEFT = 0x02, RIGHT = 0x04, BOTH = 0x08 };
enum Status { UNKNOWN_STATUS = 0x01, PLAYED = 0x02, MISSED = 0x04, EXTRA = 0x08, FAILED = 0x10, OPEN = 0x20 };

class Score
{
public:
    Score(char pitch=0);

    char pitch;
    Fraction duration;
    Fraction position;

    MidiPair midiPair;

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

    bool operator==(const Score& rhs) const {
        return (this->pitch==rhs.pitch
                && this->duration==rhs.duration
                && this->position==rhs.position
                && this->midiPair==rhs.midiPair
                && this->finger==rhs.finger
                && this->hand==rhs.hand
                && this->status==rhs.status);
    }
};


#endif // SCORE_H
