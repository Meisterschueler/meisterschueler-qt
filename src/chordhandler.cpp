#include "chordhandler.h"

ChordHandler::ChordHandler(QObject *parent) :
    QObject(parent)
{   
    // ----- Intervalle -----
    chords.insert("Unison",          "{c,c}");
    chords.insert("Minor 2nd",       "{c,d&}");
    chords.insert("Major 2nd",       "{c,d}");
    chords.insert("Minor 3rd",       "{c,e&}");
    chords.insert("Major 3rd",       "{c,e}");
    chords.insert("Perfect 4th",     "{c,f}");
    chords.insert("Augmented 4th",   "{c,f#}");
    chords.insert("Perfect 5th",     "{c,g}");
    chords.insert("Minor 6th",       "{c,a&}");
    chords.insert("Major 6th",       "{c,a}");
    chords.insert("Minor 7th",       "{c,b&}");
    chords.insert("Major 7th",       "{c,b}");
    chords.insert("Octave",          "{c,c1}");
    chords.insert("Minor 9th",       "{c,d&2}");
    chords.insert("Major 9th",       "{c,d2}");
    chords.insert("11th",            "{c,f2}");
    chords.insert("Augmented 11th",  "{c,f#2}");

    // ----- Akkorde -----
    // Dur
    chords.insert("Dur",        "{c1,e,g}");            // maj, major
    chords.insert("2",          "{c1,d,g}");            // sus2
    chords.insert("4",          "{c1,f,g}");            // sus4, m(sus4)
    chords.insert("(b5)",       "{c1,e,g&}");           // -
    chords.insert("6",          "{c1,e,g,a}");          // add6, maj6, add23
    chords.insert("maj7",       "{c1,e,g,b}");          // j7, M7, Δ
    chords.insert("maj7(b5)",   "{c1,e,g&,b}");         // -
    chords.insert("maj7(#5)",   "{c1,e,g#,b}");         // -
    chords.insert("add9",       "{c1,e,g,d2}");         // add2
    chords.insert("add21",      "{c1,e,g,f2}");         // add4
    chords.insert("6/9",        "{c1,e,g,a,d2}");       // maj6/9
    chords.insert("maj7(b9)",   "{c1,e,g,b,d&2}");      // -
    chords.insert("maj7(#9)",   "{c1,e,g,b,d#2}");      // -
    chords.insert("maj7/#11",   "{c1,e,g,b,f#2}");      // -
    chords.insert("maj7/13",    "{c1,e,g,b,a2}");       // -
    chords.insert("maj9",       "{c1,e,g,b,d2}");       // maj7/9
    chords.insert("6/9/#11",    "{c1,e&,g,a,d2,f#}");   // -
    chords.insert("maj7(#9)11", "{c1,e,g,b,d#2,f}");    // -
    chords.insert("maj9/13",    "{c1,e,g,b,d2,a}");     // maj7/9/13
    chords.insert("maj11",      "{c1,e,g,b,d2,f}");     // maj7/9/11, maj9/11

    // Moll
    chords.insert("m",          "{c1,e&,g}");           // min, minor, moll
    chords.insert("m6",         "{c1,e&,g,a}");         // min6
    chords.insert("m7",         "{c1,e&,g,b&}");        // -7, mi7, min7
    chords.insert("m(maj7)",    "{c1,e&,g,b}");         // m(j7)
    chords.insert("m(add9)",    "{c1,e&,g,d2}");        // m(add2)
    chords.insert("m(add21)",   "{c1,e&,g,f2}");        // m(add4)
    chords.insert("m6/9",       "{c1,e&,g,a,d2}");      // min6/9
    chords.insert("m7/11",      "{c1,e&,g,b&,f2}");     // min7/11
    chords.insert("m7/11(b5)",  "{c1,e&,g&,b&,f2}");    // min7/11(b5)
    chords.insert("m7/b13",     "{c1,e&,g,b&,a&2}");    // min7/b13
    chords.insert("m7/13",      "{c1,e&,g,b&,a2}");     // min7/13
    chords.insert("m(maj9)",    "{c1,e&,g,b,d2}");      // m(j9)
    chords.insert("m9",         "{c1,e&,g,b&,d}");      // min9
    chords.insert("m6/9/11",    "{c1,e&,g,a,d2,f}");    // min6/9/11
    chords.insert("m11",        "{c1,e&,g,b&,d2,f}");   // min11, m7/9/11, m9/11
    chords.insert("m13",        "{c1,e&,g,b&,d2,f,a}"); // min13

    // Dominant-Sept
    chords.insert("7",          "{c1,e,g,b&}");        // dom7
    chords.insert("7/4",        "{c1,f,g,b&}");        // 7sus4
    chords.insert("7(b5)",      "{c1,e,g&,b&}");       // Ø
    chords.insert("7(#5)",      "{c1,e,g#,b&}");       // -
    chords.insert("7(b9)",      "{c1,e,g,b&,d&2}");    // dom(b9)
    chords.insert("7(b9)4",     "{c1,f,g,b&,d&2}");    // -
    chords.insert("7(#9)",      "{c1,e,g,b&,d#2}");    // dom(#9)
    chords.insert("7/11",       "{c1,e,g,b&,f2}");     // -
    chords.insert("7/#11",      "{c1,e,g,b&,f#2}");    // -
    chords.insert("7/b13",      "{c1,e,g,b&,a&2}");    // -
    chords.insert("7/13",       "{c1,e,g,b&,a2}");     // -
    chords.insert("9",          "{c1,e,g,b&,d2}");     // dom9, 7/9
    chords.insert("9/4",        "{c1,f,g,b&,d2}");     // 9sus4, 11/4, 11sus4
    chords.insert("9(b5)",      "{c1,e,g&,b&,d2}");    // 7/9(b5), 9(#11)
    chords.insert("9(#5)",      "{c1,e,g#,b&,d2}");    // 7/9(#5)
    chords.insert("7(b9)#11",   "{c1,e,g,b&,d&2,f#}"); // -
    chords.insert("7(b9)b13",   "{c1,e,g,b&,d&2,a&}"); // -
    chords.insert("7(b9)13",    "{c1,e,g,b&,d&2,a}");  // -
    chords.insert("7(#9)#11",   "{c1,e,g,b&,d#2,f#}"); // -
    chords.insert("7(#9)b13",   "{c1,e,g,b&,d#2,a&}"); // -
    chords.insert("9/#11",      "{c1,e,g,b&,d2,f#}");  // -
    chords.insert("9/b13",      "{c1,e,g,b&,d2,a&}");  // -
    chords.insert("9/13",       "{c1,e,g,b&,d2,a}");   // 9/6, 7/9/13
    chords.insert("13/4",       "{c1,f,g,b&,d2,a}");   // 13sus4, 9/13sus4
    chords.insert("11",         "{c1,e,g,b&,d2,f}");   // dom11
    chords.insert("11(b9)",     "{c1,e,g,b&,d&2,f}");  // -
    chords.insert("11(#9)",     "{c1,e,g,b&,d#2,f}");  // -
    chords.insert("7(#9)#11",   "{c1,e,g,b&,d#2,f#}"); // -
    chords.insert("9/#11/13",   "{c1,e,g,b&,d2,f#,a}"); // -
    chords.insert("13",         "{c1,e,g,b&,d2,f,a}"); // dom13
    chords.insert("13(b9)",     "{c1,e,g,b&,d&2,f,a}"); // -
    chords.insert("13(#9)",     "{c1,e,g,b&,d#2,f,a}"); // -

    // Vermindert
    chords.insert("°",          "{c1,e&,g&}");         // dim, verm, m(b5)
    chords.insert("°7",         "{c1,e&,g&,b&&}");     // -
    chords.insert("°7/b13",     "{c1,e&,g&,b&&,a&2}"); // -

    // Halbvermindert
    chords.insert("m7(b5)",     "{c1,e&,g&,b&}");      // mØ, dim7, min7(b5)

    // Übermäßig
    chords.insert("+",          "{c1,e,g#}"); // (#5), aug, überm

    // create Umkehrungen for chords < 1 Oktave
    /*foreach (const NamedChord& chord, chords) {
        if (chord.notes.last() - chord.notes.first() < 12) {
            for (int i=1; i<chord.notes.count(); i++) {
                NamedChord modeChord = chord;
                modeChord.mode = i;
                for (int j=0; j<i; j++) {
                    modeChord.notes[j].pitch += 12;
                }
                qSort(modeChord.notes);
                chords.append(modeChord);
            }
        }
    }*/

    // Powerchords
    chords.insert("1-b3-x",     "{c1,e&}"); // -
    chords.insert("1-3-x",      "{c1,e}"); // -
    chords.insert("1-4-x",      "{c1,f}"); // -
    chords.insert("1-x-b5",     "{c1,g&}"); // -
    chords.insert("5",          "{c1,g}"); // 1-x-5
    chords.insert("1-x-6",      "{c1,a}"); // -
    chords.insert("1-x-b7",     "{c1,b&}"); // -
    chords.insert("1-x-7",      "{c1,b}"); // -

    // ----- Skalen -----
    // Kirchentonarten (Modi)
    scales.insert("ionisch",        "c d e f g a b");          // DUR I [="Dur"], N.M. III
    scales.insert("dorisch",        "c d e& f g a b&");        // DUR II, N.M. IV
    scales.insert("phrygisch",      "c d& e& f g a& b&");      // DUR III, N.M. V
    scales.insert("lydisch",        "c d e f# g a b");         // DUR IV, N.M. VI
    scales.insert("mixolydisch",    "c d e f g a b&");         // DUR V, N.M. VII
    scales.insert("äolisch",        "c d e& f g a& b&");       // DUR VI, N.M. I [="natürliches Moll"]
    scales.insert("lokrisch",       "c d& e& f g& a& b&");     // DUR VII, N.M. II
    scales.insert("äolisch 7",      "c d e& f g a& b");        // H.M. I [="harmonisches Moll"]
    scales.insert("lokrisch 6",     "c d& e& f g& a b&");      // H.M. II, lokrisch #6
    scales.insert("ionisch #5",     "c d e f g# a b");         // H.M. III
    scales.insert("dorisch #4",     "c d e& f# g a b&");       // H.M. IV
    scales.insert("phrygisch 3",    "c d& e f g a& b&");       // H.M. V, phrygisch dominant
    scales.insert("lydisch #2",     "c d# e f# g a b");        // H.M. VI
    scales.insert("lokrisch b4 bb7", "c d& e& f& g& a& b&&");  // H.M. VII
    scales.insert("äolisch 6 7",    "c d e& f g a b");         // M.M. I [="melodisches Moll"]
    scales.insert("phrygisch 6",    "c d& e& f g a b&");       // M.M. II, phrygisch #6, dorisch b2
    scales.insert("lydisch #5",     "c d e f# g# a b");        // M.M. III
    scales.insert("lydisch b7",     "c d e f# g a b&");        // M.M. IV, lydisch dominant, mixolydisch #4 (oder 11)
    scales.insert("äolisch 3",      "c d e f g a& b&");        // M.M. V, äolisch dominant, mixolydisch b6 (oder 13)
    scales.insert("lokrisch 2",     "c d e& f g& a& b&");      // M.M. VI, lokrisch #2
    scales.insert("lokrisch b4",    "c d& e& f& g& a& b&");    // M.M. VII

    // Pentatoniken
    scales.insert("Dur Pentatonik",         "c d e g a");      // diatonische Tonleiter
    scales.insert("Neutrale Pentatonik",    "c d f g b&");     // -
    scales.insert("Rock Pentatonik",        "c e& f g# b&");   // -
    scales.insert("Schottische Pentatonik", "c d f g a");      // -
    scales.insert("Moll Pentatonik",        "c e& f g b&");    // Blues Pentantonik

    // Blues-Skala
    scales.insert("Blues-Skala", "c e& f-(b5) g b&");//-
}

void ChordHandler::match() {

}

void ChordHandler::reset() {
    midiPairs.clear();
}

void ChordHandler::matchNoteOnEvent(NoteOnEvent noteOn) {
    // insert
    match();
}

void ChordHandler::matchNoteOffEvent(NoteOffEvent noteOff) {
    // insert
}

void ChordHandler::matchChannelEvents(QList<ChannelEvent> channelEvents) {
    // insert
    match();
}
