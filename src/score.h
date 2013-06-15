#ifndef SCORE_H
#define SCORE_H

#include <QSharedPointer>

#include "Fraction.h"

#include "events.h"

class MidiPair;

enum Status { UNKNOWN_STATUS = 0x01, PLAYED = 0x02, MISSED = 0x04, EXTRA = 0x08, FAILED = 0x10, OPEN = 0x20 };
enum Finger { UNKNOWN_FINGER = 0x01, THUMB = 0x02, POINTER = 0x04, MIDDLE = 0x08, RING = 0x10, LITTLE = 0x20 };
enum Hand { UNKNOWN_HAND = 0x01, LEFT = 0x02, RIGHT = 0x04, BOTH = 0x08 };

class Score
{
public:
    Score(char pitch=0, Status status=UNKNOWN_STATUS);

    unsigned char pitch;
    Fraction duration;
    Fraction position;

    MidiPair midiPair;

    Status status;
    Finger finger;
    Hand hand;
    int voice;

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
                && this->status==rhs.status
                && this->finger==rhs.finger
                && this->hand==rhs.hand
                && this->voice==rhs.voice);
    }
};
Q_DECLARE_METATYPE(Score)

#endif // SCORE_H
