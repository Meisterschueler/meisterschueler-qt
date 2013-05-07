#include <QString>
#include <QtTest>

#include "guidoservice.h"
#include "scoreservice.h"
#include "matchingservice.h"
#include "midiservice.h"
#include "needlemanwunsch.h"
#include "score.h"
#include "events.h"

class Test : public QObject
{
    Q_OBJECT
    
public:
    Test();
    
private Q_SLOTS:
    void noteEvent_comparisons();
    void noteEventPair_constructors();
    void noteEventPair_comparisons();

    void guidoService_gmnToScores_simple();
    void guidoService_gmnToScores_chord();
    void guidoService_gmnToScores_repeat();
    void guidoService_gmnToScores_finger();
    void guidoService_gmnToScores_voices();
    void guidoService_gmnToScores_duration_position();

    void scoreService_transposeStep();
    void scoreService_addFingers();
    void scoreService_concat();
    void scoreService_merge();

    void midiService_addNote();
    void midiService_saveLoad();
    void midiService_loadHanon();

    void matchingService_midiEvents2xy();

    void needlemanWunsch_emptySequencesTest();
    void needlemanWunsch_AAA_AAA_mmm_Test();
    void needlemanWunsch_AAA_ABA_mwm_Test();
    void needlemanWunsch_AAA_AZA_midm_Test();
    void needlemanWunsch_ABC_AZC_mwm_Test();
    void needlemanWunsch_AAA_AABA_mmim_Test();
    void needlemanWunsch_AABA_AAA_mmdm_Test();
    void needlemanWunsch_complexTest();
    void needlemanWunsch_matchFirstTest();
};

Test::Test()
{
}

// BASIC TYPES

void Test::noteEvent_comparisons() {
    NoteOnEvent note(0, 0, 10, 0);
    NoteOffEvent same(0, 0, 10, 0);
    NoteOffEvent higher(0, 0, 20, 0);

    QVERIFY( note == same );
    QVERIFY( note < higher );
    QVERIFY( higher > note );
}

void Test::noteEventPair_constructors() {
    NoteOnEvent A(0, 0, 10, 0);
    NoteEventPair pair1(A);
    QVERIFY( A == *(pair1.noteOn) );
    QVERIFY( NULL == pair1.noteOff );

    NoteOffEvent a(10, 0, 10, 0);
    NoteEventPair pair2(A, a);
    QVERIFY( A == *(pair2.noteOn) );
    QVERIFY( a == *(pair2.noteOff) );
}

void Test::noteEventPair_comparisons() {
    NoteEventPair pair(NoteOnEvent(0, 0, 10, 0));
    NoteEventPair same(NoteOnEvent(0, 0, 10, 0));
    NoteEventPair higher(NoteOnEvent(0, 0, 20, 0));
    NoteEventPair later(NoteOnEvent(10, 0, 10, 0));
    NoteEventPair higherAndLater(NoteOnEvent(10, 0, 20, 0));

    QVERIFY( pair == same );

    QVERIFY( pair < higher );
    QVERIFY( higher > pair );

    QVERIFY( pair < later );
    QVERIFY( later > pair );

    QVERIFY( pair < higherAndLater );
    QVERIFY( higherAndLater > pair );

    QVERIFY( higher < later );
    QVERIFY( later > higher );

    QVERIFY( higher < higherAndLater );
    QVERIFY( higherAndLater > higher );

    QVERIFY( later < higherAndLater );
    QVERIFY( higherAndLater > later );
}

// GUIDOSERVICE

void Test::guidoService_gmnToScores_simple() {
    QString gmn = "[c d e f g]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QVERIFY( notes.length() == 5 );
    QVERIFY( notes.at(0).pitch == 60 );
    QVERIFY( notes.at(1).pitch == 62 );
    QVERIFY( notes.at(2).pitch == 64 );
    QVERIFY( notes.at(3).pitch == 65 );
    QVERIFY( notes.at(4).pitch == 67 );
}

void Test::guidoService_gmnToScores_chord() {
    QString gmn = "[ {c,e,g} ]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QVERIFY( notes.length() == 3 );
    QVERIFY( notes.at(0).pitch == 60 );
    QVERIFY( notes.at(1).pitch == 64 );
    QVERIFY( notes.at(2).pitch == 67 );
}

void Test::guidoService_gmnToScores_repeat() {
    QString gmn = "[c d \\beginRepeat e f g \\endRepeat a]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QVERIFY( notes.length() == 9 );
}

void Test::guidoService_gmnToScores_finger() {
    QString gmn = "[c \\finger<\"3\"> e \\finger<\"1\"> g]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QVERIFY( notes.length() == 3 );
    QVERIFY( notes.at(0).finger == UNKNOWN_FINGER );
    QVERIFY( notes.at(1).finger == MIDDLE );
    QVERIFY( notes.at(2).finger == THUMB );
}

void Test::guidoService_gmnToScores_voices() {
    QString gmn = "{[c e g],[d f g]}";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QVERIFY( notes.length() == 6 );
}

void Test::guidoService_gmnToScores_duration_position() {
    QString gmn = "[c d/8 e/2 _/4 g/8]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QVERIFY( notes.length() == 4 );
    QVERIFY( notes.at(0).position == Fraction(0, 4) );
    QVERIFY( notes.at(0).duration == Fraction(1, 4) );

    QVERIFY( notes.at(1).position == Fraction(1, 4) );
    QVERIFY( notes.at(1).duration == Fraction(1, 8) );

    QVERIFY( notes.at(2).position == Fraction(3, 8) );
    QVERIFY( notes.at(2).duration == Fraction(1, 2) );

    QVERIFY( notes.at(3).position == Fraction(9, 8) );
    QVERIFY( notes.at(3).duration == Fraction(1, 8) );
}

// SCORESERVICE

void Test::scoreService_transposeStep() {
    QString gmn = "[c0 e g c1]";
    QList<Score> ionisch = GuidoService::gmnToScores(gmn);
    QVERIFY( ionisch.size() == 4 );
    QVERIFY( ionisch.at(0).pitch == 48 );
    QVERIFY( ionisch.at(1).pitch == 52 );
    QVERIFY( ionisch.at(2).pitch == 55 );
    QVERIFY( ionisch.at(3).pitch == 60 );

    QList<Score> dorisch = ScoreService::transposeStep(ionisch, 1);
    QVERIFY( dorisch.size() == 4 );
    QVERIFY( dorisch.at(0).pitch == 50 );
    QVERIFY( dorisch.at(1).pitch == 53 );
    QVERIFY( dorisch.at(2).pitch == 57 );
    QVERIFY( dorisch.at(3).pitch == 62 );

    QList<Score> mixolydisch = ScoreService::transposeStep(ionisch, 4);
    QVERIFY( mixolydisch.size() == 4 );
    QVERIFY( mixolydisch.at(0).pitch == 55 );
    QVERIFY( mixolydisch.at(1).pitch == 59 );
    QVERIFY( mixolydisch.at(2).pitch == 62 );
    QVERIFY( mixolydisch.at(3).pitch == 67 );

    QList<Score> aeolisch = ScoreService::transposeStep(ionisch, -2);
    QVERIFY( aeolisch.size() == 4 );
    QVERIFY( aeolisch.at(0).pitch == 45 );
    QVERIFY( aeolisch.at(1).pitch == 48 );
    QVERIFY( aeolisch.at(2).pitch == 52 );
    QVERIFY( aeolisch.at(3).pitch == 57 );
}

void Test::scoreService_addFingers() {
    QString gmn = "[c d e f g]";
    QList<Score> scores = GuidoService::gmnToScores(gmn);
    QVector<int> fingers {1, 2, 3};

    QList<Score> fingeredScores = ScoreService::addFingers(scores, fingers);

    QVERIFY( fingeredScores.size() == 5 );
    QVERIFY( fingeredScores.at(0).finger == 1 );
    QVERIFY( fingeredScores.at(1).finger == 2 );
    QVERIFY( fingeredScores.at(2).finger == 3 );
    QVERIFY( fingeredScores.at(3).finger == 1 );
    QVERIFY( fingeredScores.at(4).finger == 2 );
}

void Test::scoreService_concat() {
    QString gmn = "[c0/4 d e]";
    QList<Score> scores1 = GuidoService::gmnToScores(gmn);
    QList<Score> scores2 = GuidoService::gmnToScores(gmn);

    QList<Score> scores3 = ScoreService::concat(scores1, scores2);
    QVERIFY( scores3.size() == 6 );

    Fraction nextPosition;
    for (Score score : scores3) {
        Fraction position = score.position;
        QVERIFY( nextPosition == position );
        nextPosition = position + score.duration;
    }
}

void Test::scoreService_merge() {
    QString rightGmn = "[c0/4 g/8 f {c/4,e}]";
    QString leftGmn = "[_/8 c-1 {b-2,d-1} g1 c-1]";
    QList<Score> rightScores = GuidoService::gmnToScores(rightGmn);
    QList<Score> leftScores = GuidoService::gmnToScores(leftGmn);
    QList<Score> merged = ScoreService::merge(rightScores, leftScores);

    QVERIFY( merged.size() == 10);

    QVERIFY( merged.at(0).pitch == 48 );
    QVERIFY( merged.at(1).pitch == 36 );
    QVERIFY( merged.at(2).pitch == 35 );
    QVERIFY( merged.at(3).pitch == 38 );
    QVERIFY( merged.at(4).pitch == 55 );
    QVERIFY( merged.at(5).pitch == 53 );
    QVERIFY( merged.at(6).pitch == 67 );
    QVERIFY( merged.at(7).pitch == 36 );
    QVERIFY( merged.at(8).pitch == 48 );
    QVERIFY( merged.at(9).pitch == 52 );
}

// MIDISERVICE

void Test::midiService_addNote() {
    // AaBbEFCecf | GHgDhd

    NoteOnEvent A(  0, 0, 10, 0); NoteOffEvent a( 50, 0, 10, 0);
    NoteOnEvent B(100, 0, 20, 0); NoteOffEvent b(150, 0, 20, 0);
    NoteOnEvent C(200, 0, 30, 0); NoteOffEvent c(250, 0, 30, 0);
    NoteOnEvent D(300, 0, 40, 0); NoteOffEvent d(350, 0, 40, 0);
    NoteOnEvent E(400, 0, 60, 0); NoteOffEvent e(450, 0, 60, 0);
    NoteOnEvent F(500, 0, 70, 0); NoteOffEvent f(550, 0, 70, 0);
    NoteOnEvent G(600, 0, 80, 0); NoteOffEvent g(650, 0, 80, 0);
    NoteOnEvent H(700, 0, 90, 0); NoteOffEvent h(750, 0, 90, 0);

    QList<NoteEventPair> pairs;

    MidiService::addNoteOn(pairs, A);
    MidiService::addNoteOff(pairs, a);
    MidiService::addNoteOn(pairs, B);
    MidiService::addNoteOff(pairs, b);
    MidiService::addNoteOn(pairs, E);
    MidiService::addNoteOn(pairs, F);
    MidiService::addNoteOn(pairs, C);
    MidiService::addNoteOff(pairs, e);
    MidiService::addNoteOff(pairs, c);
    MidiService::addNoteOff(pairs, f);

    MidiService::addNoteOn(pairs, G);
    MidiService::addNoteOn(pairs, H);
    MidiService::addNoteOff(pairs, g);
    MidiService::addNoteOn(pairs, D);
    MidiService::addNoteOff(pairs, h);
    MidiService::addNoteOff(pairs, d);

    QVERIFY( pairs.size() == 8 );

    QVERIFY( A == *pairs.at(0).noteOn );
    QVERIFY( a == *pairs.at(0).noteOff );
    QVERIFY( B == *pairs.at(1).noteOn );
    QVERIFY( b == *pairs.at(1).noteOff );
    QVERIFY( C == *pairs.at(2).noteOn );
    QVERIFY( c == *pairs.at(2).noteOff );
    QVERIFY( D == *pairs.at(3).noteOn );
    QVERIFY( d == *pairs.at(3).noteOff );
    QVERIFY( E == *pairs.at(4).noteOn );
    QVERIFY( e == *pairs.at(4).noteOff );
    QVERIFY( F == *pairs.at(5).noteOn );
    QVERIFY( f == *pairs.at(5).noteOff );
    QVERIFY( G == *pairs.at(6).noteOn );
    QVERIFY( g == *pairs.at(6).noteOff );
    QVERIFY( H == *pairs.at(7).noteOn );
    QVERIFY( h == *pairs.at(7).noteOff );
}

void Test::midiService_saveLoad() {
    NoteOnEvent A(  0, 0, 48, 0); NoteOffEvent a( 50, 0, 48, 0);
    NoteOnEvent B(100, 0, 52, 0); NoteOffEvent b(150, 0, 52, 0);
    NoteOnEvent C(200, 0, 55, 0); NoteOffEvent c(250, 0, 55, 0);

    QList<NoteEventPair> saveit;

    MidiService::addNoteOn(saveit, A);
    MidiService::addNoteOff(saveit, a);
    MidiService::addNoteOn(saveit, B);
    MidiService::addNoteOff(saveit, b);
    MidiService::addNoteOn(saveit, C);
    MidiService::addNoteOff(saveit, c);

    QTemporaryFile tempFile("midiService");
    tempFile.open();
    tempFile.close();

    MidiService::save(tempFile.fileName(), saveit);

    QList<NoteEventPair> loadit = MidiService::load(tempFile.fileName());

    QVERIFY( loadit.size() == saveit.size() );
    for (int i = 0; i < loadit.size(); ++i) {
        QVERIFY( loadit.at(i) == saveit.at(i) );
    }
}

void Test::midiService_loadHanon() {
    QString fileName1("../../meisterschueler/test/midifiles/hanonNo1Left.mid");
    QString fileName2("../../meisterschueler/test/midifiles/hanonNo1Both.mid");
    QFile hanonFile1(fileName1);
    QFile hanonFile2(fileName2);
    QVERIFY( hanonFile1.exists() );
    QVERIFY( hanonFile2.exists() );

    QList<NoteEventPair> loadit;
    loadit = MidiService::load(fileName1);
    QVERIFY( loadit.size() == 29*8 );

    loadit = MidiService::load(fileName2);
    QVERIFY( loadit.size() == 29*8*2 );
}

// MATCHINGSERVICE

void Test::matchingService_midiEvents2xy() {
    NoteOnEvent A(  0, 0, 64, 0); NoteOffEvent a( 50, 0, 64, 0);
    NoteOnEvent B(100, 0, 71, 0); NoteOffEvent b(150, 0, 71, 0);
    NoteOnEvent C(200, 0, 66, 0); NoteOffEvent c(250, 0, 66, 0);
    NoteOnEvent D(200, 0, 71, 0); NoteOffEvent d(250, 0, 71, 0);

    NoteEventPair Aa(A, a);
    NoteEventPair Bb(B, b);
    NoteEventPair Cc(C);
    NoteEventPair Dd(D, d);

    QList<NoteEventPair> pairs;
    pairs.append(Aa);
    pairs.append(Bb);
    pairs.append(Cc);
    pairs.append(Dd);

    QString pitchSequence = MatchingService::midiEvents2pitchSequence(pairs);
    QVERIFY( pitchSequence.length() == 4 );
    QVERIFY( pitchSequence.at(0).toLatin1() == 64 );
    QVERIFY( pitchSequence.at(1).toLatin1() == 71 );
    QVERIFY( pitchSequence.at(2).toLatin1() == 66 );
    QVERIFY( pitchSequence.at(3).toLatin1() == 71 );

    QString intervalSequence = MatchingService::midiEvents2intervalSequence(pairs);
    QVERIFY( intervalSequence.length() == 3 );
    QVERIFY( intervalSequence.at(0).toLatin1() == 7 );
    QVERIFY( intervalSequence.at(1).toLatin1() == -5 );
    QVERIFY( intervalSequence.at(2).toLatin1() == 5 );

    QString pressedSequence = MatchingService::midiEvents2pressedSequence(pairs);
    QVERIFY( pressedSequence.length() == 4 );
    QVERIFY( pressedSequence.compare("..X.") == 0 );
}

// NEEDLEMANWUNSCH

void Test::needlemanWunsch_emptySequencesTest() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("", "AAA");
    QVERIFY("iii" == alignment);

    alignment = matcher.getAlignments("AAA", "");
    QVERIFY("ddd" == alignment);

    alignment = matcher.getAlignments("", "");
    QVERIFY("" == alignment);
}

void Test::needlemanWunsch_AAA_AAA_mmm_Test() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("AAA", "AAA");
    QVERIFY("mmm" == alignment);
}

void Test::needlemanWunsch_AAA_ABA_mwm_Test() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("AAA", "ABA");
    QVERIFY("mwm" == alignment);
}

void Test::needlemanWunsch_AAA_AZA_midm_Test() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("AAA", "AZA");
    //QVERIFY( "midm" == alignment);	// 1. Wahl
    QVERIFY("mimd" ==alignment);	// 2. Wahl (auch nicht falsch, aber unschön)
}

void Test::needlemanWunsch_ABC_AZC_mwm_Test() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("ABC", "AZC");
    QVERIFY("mwm" == alignment);
}

void Test::needlemanWunsch_AAA_AABA_mmim_Test() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("AAA", "AABA");
    QVERIFY("mmim" == alignment);
}

void Test::needlemanWunsch_AABA_AAA_mmdm_Test() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("AABA", "AAA");
    QVERIFY("mmdm" == alignment);
}

void Test::needlemanWunsch_complexTest() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("BCEFGH", "ABCDFG");
    QVERIFY("immwmmd" == alignment );
}

void Test::needlemanWunsch_matchFirstTest() {
    NeedlemanWunsch matcher;
    QString alignment = matcher.getAlignments("AAA", "AAAA");
    QVERIFY("mmmi" == alignment);

    alignment = matcher.getAlignments("AAAA", "AAA");
    QVERIFY("mmmd" == alignment);
}

QTEST_APPLESS_MAIN(Test)

#include "tst_test.moc"
