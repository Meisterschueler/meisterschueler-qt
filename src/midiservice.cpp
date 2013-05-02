#include "midiservice.h"

#include "events.h"
#include <QSharedPointer>
#include <QMutableListIterator>

#include "jdksmidi/world.h"
#include "jdksmidi/track.h"
#include "jdksmidi/multitrack.h"
#include "jdksmidi/filereadmultitrack.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/fileshow.h"
#include "jdksmidi/filewritemultitrack.h"

MidiService::MidiService()
{
}

void MidiService::addNoteOn(QList<NoteEventPair>& pairs, NoteOnEvent noteOn) {
    QSharedPointer<NoteOnEvent> pointer(new NoteOnEvent(noteOn));
    NoteEventPair pair;
    pair.noteOn = pointer;
    pairs.append(pair);
    qSort(pairs);
}

void MidiService::addNoteOff(QList<NoteEventPair>& pairs, NoteOffEvent noteOff) {
    QSharedPointer<NoteOffEvent> pointer(new NoteOffEvent(noteOff));
    QMutableListIterator<NoteEventPair> it(pairs);
    while (it.hasNext()) {
        it.next();
        if (it.value().noteOn->getNote() == pointer->getNote() && !it.value().noteOff) {
            it.value().noteOff = pointer;
            break;
        }
    }
}

using namespace jdksmidi;
using namespace std;

void MidiService::save(const QString& fileName, const QList<NoteEventPair>& pairs) {
    QList<QSharedPointer<NoteEvent>> events;
    for (NoteEventPair pair : pairs) {
        events.append(pair.noteOn);
        if (pair.noteOff) {
            events.append(pair.noteOff);
        }
    }

    MIDITimedBigMessage m; // the object for individual midi events
    unsigned char chan, note, velocity;

    MIDIClockTime t; // time in midi ticks
    MIDIClockTime dt = 100; // time interval (1 second)
    int clks_per_beat = 100; // number of ticks in crotchet (1...32767)
    int num_tracks = 2; // tracks 0 and 1

    MIDIMultiTrack tracks( num_tracks );  // the object which will hold all the tracks
    tracks.SetClksPerBeat( clks_per_beat );
    int trk; // track number, 0 or 1

    t = 0;
    m.SetTime( t );

    // track 0 is used for tempo and time signature info, and some other stuff
    trk = 0;

    /*
          SetTimeSig( numerator, denominator_power )
          The numerator is specified as a literal value, the denominator_power is specified as (get ready!)
          the value to which the power of 2 must be raised to equal the number of subdivisions per whole note.

          For example, a value of 0 means a whole note because 2 to the power of 0 is 1 (whole note),
          a value of 1 means a half-note because 2 to the power of 1 is 2 (half-note), and so on.

          (numerator, denominator_power) => musical measure conversion
          (1, 1) => 1/2
          (2, 1) => 2/2
          (1, 2) => 1/4
          (2, 2) => 2/4
          (3, 2) => 3/4
          (4, 2) => 4/4
          (1, 3) => 1/8
        */

    m.SetTimeSig( 4, 2 ); // measure 4/4 (default values for time signature)
    tracks.GetTrack( trk )->PutEvent( m );

    int tempo = 1000000; // set tempo to 1 000 000 usec = 1 sec in crotchet
    // with value of clks_per_beat (100) result 10 msec in 1 midi tick
    // If no tempo is define, 120 beats per minute is assumed.

    // m.SetTime( t ); // do'nt need, because previous time is not changed
    m.SetTempo( tempo );
    tracks.GetTrack( trk )->PutEvent( m );

    // META_TRACK_NAME text in track 0 music notation software like Sibelius uses as headline of the music
    tracks.GetTrack( trk )->PutTextEvent(t, META_TRACK_NAME, "LibJDKSmidi create_midifile.cpp example by VRM");

    // create cannal midi events and add them to a track 1

    trk = 1;

    // META_TRACK_NAME text in tracks >= 1 Sibelius uses as instrument name (left of staves)
    tracks.GetTrack( trk )->PutTextEvent(t, META_TRACK_NAME, "Church Organ");

    // create individual midi events with the MIDITimedBigMessage and add them to a track 1

    t = 0;

    for (QSharedPointer<NoteEvent> event : events) {
        m.SetTime( (*event).getTime() );
        if ((*event).type() == QEvent::User + STATUS_NOTEON) {
            m.SetNoteOn( chan = 0, note = (*event).getNote(), velocity = (*event).getVelocity() );
            tracks.GetTrack( trk )->PutEvent( m );
        } else if ((*event).type() == QEvent::User + STATUS_NOTEOFF) {
            m.SetNoteOff( chan = 0, note = (*event).getNote(), velocity = (*event).getVelocity() );
            tracks.GetTrack( trk )->PutEvent( m );
        }
    }

    tracks.SortEventsOrder();

    const char *outfile_name = fileName.toLatin1();
    MIDIFileWriteStreamFileName out_stream( outfile_name );

    if( out_stream.IsValid() ) {
        // the object which takes the midi tracks and writes the midifile to the output stream
        MIDIFileWriteMultiTrack writer( &tracks, &out_stream );

        // write the output file
        if ( writer.Write( num_tracks ) ) {
            cout << "\nOK writing file " << outfile_name << endl;
        } else {
            cerr << "\nError writing file " << outfile_name << endl;
        }
    } else {
        cerr << "\nError opening file " << outfile_name << endl;
    }
}

QList<NoteEventPair> parseMIDIMultiTrack ( MIDIMultiTrack *mlt ) {
    QList<NoteEventPair> result;

    MIDIMultiTrackIterator i ( mlt );
    const MIDITimedBigMessage *msg;
    i.GoToTime ( 0 );

    do {
        int trk_num;

        if ( i.GetCurEvent ( &trk_num, &msg ) ) {
            if (msg->IsNoteOn()) {
                MidiService::addNoteOn(result, NoteOnEvent(msg->GetTime(), msg->GetChannel(), msg->GetNote(), msg->GetVelocity()));
            } else if (msg->IsNoteOff()) {
                MidiService::addNoteOff(result, NoteOffEvent(msg->GetTime(), msg->GetChannel(), msg->GetNote(), msg->GetVelocity()));
            }
        }
    } while ( i.GoToNextEvent() );

    return result;
}

QList<NoteEventPair> MidiService::load(const QString fileName) {
    QList<NoteEventPair> result;

    MIDIFileReadStreamFile rs ( fileName.toLatin1() );
    MIDIMultiTrack tracks;
    MIDIFileReadMultiTrack track_loader ( &tracks );
    MIDIFileRead reader ( &rs, &track_loader );
    reader.Parse();
    result = parseMIDIMultiTrack ( &tracks );

    return result;
}
