#include "midiservice.h"

#include "events.h"

#include <QDebug>
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

int MidiService::addNoteOn(QList<MidiPairCluster>& pairClusters, const NoteOnEvent &noteOn) {
    MidiPair pair(noteOn);
    if (pairClusters.isEmpty()) {
        pairClusters.append(pair);
        return 0;
    }

    int end = pairClusters.count()-1;
    if (pairClusters.at(end).time+50 < noteOn.getTime()) {
        pairClusters.append(MidiPair(noteOn));
        return end+1;
    } else {
        pairClusters[end].midiPairs.append(MidiPair(noteOn));
        qSort(pairClusters[end].midiPairs);
        return end;
    }
}

int MidiService::addNoteOff(QList<MidiPairCluster>& pairClusters, const NoteOffEvent &noteOff) {
    for (int idx = pairClusters.count()-1; idx>=0; idx--) {
        MidiPairCluster *currentCluster = &pairClusters[idx];
        for (int idx2 = 0; idx2 < currentCluster->midiPairs.count(); idx2++) {
            if (currentCluster->midiPairs.at(idx2).noteOn.getNote() == noteOff.getNote() && currentCluster->midiPairs.at(idx2).noteOff == emptyNoteOffEvent) {
                currentCluster->midiPairs[idx2].noteOff = noteOff;
                return idx;
            }
        }
    }

    qDebug() << "WTF!!!";
    return 0;
}

using namespace jdksmidi;
using namespace std;

bool MidiService::save(const QString& fileName, const QList<ChannelEvent>& events) {

    Q_ASSERT( events.count() > 0 );

    MIDITimedBigMessage m; // the object for individual midi events
    unsigned char chan, note, velocity;

    MIDIClockTime t; // time in midi ticks
    //MIDIClockTime dt = 100; // time interval (1 second)
    int clks_per_beat = 1000; // number of ticks in crotchet (1...32767)
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

    for (ChannelEvent event : events) {
        m.SetTime( event.getTime() );
        if (event.type() == Event::NoteOnEventType) {
            m.SetNoteOn( chan = event.getChannel(), note = event.getNote(), velocity = event.getVelocity() );
            tracks.GetTrack( trk )->PutEvent( m );
        } else if (event.type() == Event::NoteOffEventType) {
            m.SetNoteOff( chan = event.getChannel(), note = event.getNote(), velocity = event.getVelocity() );
            tracks.GetTrack( trk )->PutEvent( m );
        }
    }

    tracks.SortEventsOrder();

    const char *outfile_name = fileName.toLatin1();
    MIDIFileWriteStreamFileName out_stream( outfile_name );

    bool success = false;
    if( out_stream.IsValid() ) {
        // the object which takes the midi tracks and writes the midifile to the output stream
        MIDIFileWriteMultiTrack writer( &tracks, &out_stream );

        // write the output file
        if ( writer.Write( num_tracks ) ) {
            success = true;
        } else {
            cerr << "\nError writing file " << outfile_name << endl;
        }
    } else {
        cerr << "\nError opening file " << outfile_name << endl;
    }
    return success;
}

QList<ChannelEvent> parseMIDIMultiTrack ( MIDIMultiTrack *mlt ) {
    QList<ChannelEvent> result;

    MIDIMultiTrackIterator i ( mlt );
    const MIDITimedBigMessage *msg;
    i.GoToTime ( 0 );

    do {
        int trk_num;

        if ( i.GetCurEvent ( &trk_num, &msg ) ) {
            if (msg->IsNoteOn()) {
                result.append(NoteOnEvent(msg->GetTime(), msg->GetChannel(), msg->GetNote(), msg->GetVelocity()));
            } else if (msg->IsNoteOff()) {
                result.append(NoteOffEvent(msg->GetTime(), msg->GetChannel(), msg->GetNote(), msg->GetVelocity()));
            }
        }
    } while ( i.GoToNextEvent() );

    return result;
}

QList<ChannelEvent> MidiService::load(const QString fileName) {
    QList<ChannelEvent> result;

    MIDIFileReadStreamFile rs ( fileName.toLatin1() );
    MIDIMultiTrack tracks;
    MIDIFileReadMultiTrack track_loader ( &tracks );
    MIDIFileRead reader ( &rs, &track_loader );
    reader.Parse();
    result = parseMIDIMultiTrack ( &tracks );

    return result;
}
