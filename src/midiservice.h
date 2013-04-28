#ifndef MIDISERVICE_H
#define MIDISERVICE_H

#include <QList>

class NoteEventPair;
class NoteOnEvent;
class NoteOffEvent;

class MidiService
{
public:
    MidiService();

    static void addNoteOn(QList<NoteEventPair> &pairs, NoteOnEvent noteOn);
    static void addNoteOff(QList<NoteEventPair> &pairs, NoteOffEvent noteOff);
};

#endif // MIDISERVICE_H
