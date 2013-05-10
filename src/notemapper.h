#ifndef NOTEMAPPER_H
#define NOTEMAPPER_H

#include "TimeUnwrap.h"
#include "Fraction.h"

#include <QString>
#include <QList>

class Score;

class NoteMapper : public TimeUnwrap
{
private:
    QList<Score> notes;
    Fraction position;

public:
    NoteMapper(ARMusicalVoice *voice);
    QList<Score> getNotes();

protected:
    virtual void Event (const ARMusicalObject * ev, EventType type);
    virtual void Note (const ARMusicalObject * ev);
    virtual void Rest (const ARMusicalObject * ev);
};

#endif // NOTEMAPPER_H
