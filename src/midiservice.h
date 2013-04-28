#ifndef MIDISERVICE_H
#define MIDISERVICE_H

#include <QList>

class NoteEventPair;
class NoteEvent;

class MidiService
{
public:
    MidiService();

    static void addNote(QList<NoteEventPair> pairs, NoteEvent event);
};

#endif // MIDISERVICE_H
