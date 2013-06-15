#include "notemapper.h"

#include <QDebug>

#include "ARMusicalObject.h"
#include "ARNote.h"
#include "ARRest.h"

#include "score.h"

NoteMapper::NoteMapper(ARMusicalVoice *voice) : TimeUnwrap(voice)
{
}

QList<Score> NoteMapper::getNotes() const {
    return notes;
}

void NoteMapper::Event(const ARMusicalObject *ev, EventType type) {
    /*switch (type) {
    case kNote:
        qDebug("Event: Note");
        break;
    case kRest:
        qDebug("Event: Rest");
        break;
    case kRepeatBegin:
        qDebug("Event: RepeatBegin");
        break;
    case kRepeatEnd:
        qDebug("Event: RepeatEnd");
        break;
    default:
        qDebug("Event: %i", type);
        break;
    }*/

    TimeUnwrap::Event(ev, type);
}

void NoteMapper::Note(const ARMusicalObject *ev) {
    const ARNote *arn = dynamic_cast<const ARNote*>(ev);
    Score score;
    score.pitch = arn->midiPitch();
    score.duration = arn->getDuration();
    score.position = position;

    score.voice = arn->getVoiceNum();

    position += arn->getDuration();

    if (score.pitch == 255) {
        qDebug("Invalid pitch");
    } else {
        notes.append(score);
    }

    TimeUnwrap::Note(ev);
}

void NoteMapper::Rest(const ARMusicalObject *ev) {
    TimeUnwrap::Rest(ev);

    const ARRest *arr = dynamic_cast<const ARRest*>(ev);
    position += arr->getDuration();
    //qDebug("Rest");
}
