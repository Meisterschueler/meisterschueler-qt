#include <QString>
#include <QtTest>

#include <QDebug>

#include "guidoservice.h"
#include "hanonsongfactory.h"
#include "scoreservice.h"
#include "matchingservice.h"
#include "matchinghandler.h"
#include "midiservice.h"
#include "midiwrapper.h"
#include "needlemanwunsch.h"
#include "playbackhandler.h"
#include "score.h"
#include "songservice.h"
#include "statisticsservice.h"
#include "events.h"

class Test : public QObject
{
    Q_OBJECT
    
public:
    Test();
    
private Q_SLOTS:
    void channelEvent_comparisons();
    void noteOnEvent_comparisons();
    void midiPair_constructors();
    void midiPair_comparisons_single();
    void midiPair_comparisons_chord();
    void song_comparisons();
    void matchingItem_comparisons();

    void needlemanWunsch_emptySequencesTest();
    void needlemanWunsch_AAA_AAA_mmm_Test();
    void needlemanWunsch_AAA_ABA_mwm_Test();
    void needlemanWunsch_AAA_AZA_midm_Test();
    void needlemanWunsch_ABC_AZC_mwm_Test();
    void needlemanWunsch_AAA_AABA_mmim_Test();
    void needlemanWunsch_AABA_AAA_mmdm_Test();
    void needlemanWunsch_complexTest();
    void needlemanWunsch_matchFirstTest();
    void needlemanWunsch_intervals();
    void needlemanWunsch_transposition();

    void guidoService_gmnToScores_simple();
    void guidoService_gmnToScores_chord();
    void guidoService_gmnToScores_repeat();
    void guidoService_gmnToScores_finger();
    void guidoService_gmnToScores_voices();
    void guidoService_gmnToScores_duration_position();

    void scoreService_transposeStep();
    void scoreService_transposeSteps();
    void scoreService_addFingers();
    void scoreService_concat();
    void scoreService_merge();
    void scoreService_scoresToXYSequence();
    void scoreService_filterFinger();
    void scoreService_filterStatus();

    void songFactories_hanon();

    void midiService_addNote();
    void midiService_saveLoad();
    void midiService_loadHanon();

    void matchingService_midiPairs2xy();
    void matchingService_getSaveAlignment();
    void matchingService_getAlignment_prunning();
    void matchingService_getTransposition();
    void matchingService_getQuality();
    void matchingService_isFinished();
    void matchingService_cutMatchingMidiPairs();
    void matchingService_merge();

    void matchingHandler_simple();
    void matchingHandler_hanonNo1Left();

    void statisticsService_statisticItem();
    void statisticsService_statisticCluster();

    void playbackHandler_simple();

    void midiWrapper_simple();

private:
    MatchingItem gmnToMatchingItem(const QString &gmn);
};

Q_DECLARE_METATYPE(MatchingItem)

Test::Test()
{
    qRegisterMetaType<Fraction>("Fraction");
    qRegisterMetaType<MatchingItem>("MatchingItem");
    qRegisterMetaType<NoteOnEvent>("NoteOnEvent");
    qRegisterMetaType<NoteOffEvent>("NoteOffEvent");
}

// BASIC TYPES

void Test::channelEvent_comparisons() {
    ChannelEvent event(100, 0, 0, 0, NoteOnEventType);
    ChannelEvent earlier(50, 0, 0, 0, NoteOnEventType);
    ChannelEvent later(150, 0, 0, 0, NoteOnEventType);

    QVERIFY( earlier < event );
    QVERIFY( event < later );
}

void Test::noteOnEvent_comparisons() {
    NoteOnEvent note(0, 0, 10, 0);
    NoteOnEvent same(0, 0, 10, 0);
    NoteOnEvent higher(0, 0, 20, 0);

    QVERIFY( note == same );
    QVERIFY( note < higher );
}

void Test::midiPair_constructors() {
    NoteOnEvent A(0, 0, 10, 0);
    MidiPair pair1(A);
    QVERIFY( A == *(pair1.noteOn) );
    QVERIFY( NULL == pair1.noteOff );

    NoteOffEvent a(10, 0, 10, 0);
    MidiPair pair2(A, a);
    QVERIFY( A == *(pair2.noteOn) );
    QVERIFY( a == *(pair2.noteOff) );
}

void Test::midiPair_comparisons_single() {
    MidiPair pair(NoteOnEvent(0, 0, 10, 0));
    MidiPair same(NoteOnEvent(0, 0, 10, 0));
    MidiPair higher(NoteOnEvent(0, 0, 20, 0));
    MidiPair lower(NoteOnEvent(0, 0, 5, 0));
    MidiPair later(NoteOnEvent(100, 0, 10, 0));
    MidiPair higherAndLater(NoteOnEvent(100, 0, 20, 0));
    MidiPair lowerAndLater(NoteOnEvent(100, 0, 5, 0));

    QVERIFY( pair == same );


    QVERIFY( pair < higher );
    QCOMPARE( higher < pair, false );

    QVERIFY( lower < pair );
    QCOMPARE( pair < lower, false );

    QVERIFY( pair < later );
    QCOMPARE( later < pair, false );

    QVERIFY( pair < higherAndLater );
    QCOMPARE( higherAndLater < pair, false );

    QVERIFY( pair < lowerAndLater );
    QCOMPARE( lowerAndLater < pair, false );


    QVERIFY( higher < later );
    QCOMPARE( later < higher, false );

    QVERIFY( lower < higher );
    QCOMPARE( higher < lower, false );

    QVERIFY( higher < higherAndLater );
    QCOMPARE( higherAndLater < higher, false );

    QVERIFY( higher < lowerAndLater );
    QCOMPARE( lowerAndLater < higher, false );


    QVERIFY( lower < later );
    QCOMPARE( later < lower, false );

    QVERIFY( lower < higherAndLater );
    QCOMPARE( higherAndLater < lower, false );

    QVERIFY( lower < lowerAndLater );
    QCOMPARE( lowerAndLater < lower, false );


    QVERIFY( later < higherAndLater );
    QCOMPARE( higherAndLater < later, false );

    QVERIFY( lowerAndLater < later );
    QCOMPARE( later < lowerAndLater, false );


    QVERIFY( lowerAndLater < higherAndLater );
    QCOMPARE( higherAndLater < lowerAndLater, false );
}

void Test::midiPair_comparisons_chord() {
    MidiPair chord1note1(NoteOnEvent(0, 0, 30, 0));
    MidiPair chord1note2(NoteOnEvent(10, 0, 10, 0));
    MidiPair chord1note3(NoteOnEvent(20, 0, 20, 0));

    MidiPair chord2note1(NoteOnEvent(100, 0, 20, 0));
    MidiPair chord2note2(NoteOnEvent(110, 0, 30, 0));

    //QVERIFY( chord1note1 > chord1note2 );
    QVERIFY( chord1note2 < chord1note1 );
    //QVERIFY( chord1note1 > chord1note3 );
    QVERIFY( chord1note3 < chord1note1 );
    QVERIFY( chord1note1 < chord2note1 );
    QVERIFY( chord1note1 < chord2note2 );

    QVERIFY( chord1note2 < chord1note3 );
    QVERIFY( chord1note2 < chord2note1 );
    QVERIFY( chord1note2 < chord2note2 );

    QVERIFY( chord1note3 < chord2note1 );
    QVERIFY( chord1note3 < chord2note2 );

    QVERIFY( chord2note1 < chord2note2 );
}

void Test::song_comparisons() {
    Song song1;
    song1.id = 01L;

    Song song2;
    song2.id = 01L;

    Song song3;
    song3.id = 02L;

    QVERIFY( song1 == song2 );
    QVERIFY( song1 != song3 );
}

void Test::matchingItem_comparisons() {
    MatchingItem item1;
    item1.quality = 0.5;

    MatchingItem item2;
    item2.quality = 0.2;

    QVERIFY( item1 < item2 );
    QVERIFY( item2 > item1 );
}

// NEEDLEMANWUNSCH

void Test::needlemanWunsch_emptySequencesTest() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("", "AAA");
    QVERIFY( "iii" == alignment );

    alignment = matcher.getAlignments("AAA", "");
    QVERIFY( "ddd" == alignment );

    alignment = matcher.getAlignments("", "");
    QVERIFY( "" == alignment );
}

void Test::needlemanWunsch_AAA_AAA_mmm_Test() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("AAA", "AAA");
    QVERIFY( "mmm" == alignment );
}

void Test::needlemanWunsch_AAA_ABA_mwm_Test() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("AAA", "ABA");
    QVERIFY( "mwm" == alignment );
}

void Test::needlemanWunsch_AAA_AZA_midm_Test() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("AAA", "AZA");
    //QVERIFY( "midm" == alignment );	// 1. Wahl
    QVERIFY( "mimd" == alignment );	// 2. Wahl (auch nicht falsch, aber unschön)
}

void Test::needlemanWunsch_ABC_AZC_mwm_Test() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("ABC", "AZC");
    QVERIFY( "mwm" == alignment );
}

void Test::needlemanWunsch_AAA_AABA_mmim_Test() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("AAA", "AABA");
    QVERIFY( "mmim" == alignment );
}

void Test::needlemanWunsch_AABA_AAA_mmdm_Test() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("AABA", "AAA");
    QVERIFY( "mmdm" == alignment );
}

void Test::needlemanWunsch_complexTest() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("BCEFGH", "ABCDFG");
    QVERIFY( "immwmmd" == alignment );
}

void Test::needlemanWunsch_matchFirstTest() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("AAA", "AAAA");
    QVERIFY( "mmmi" == alignment );

    alignment = matcher.getAlignments("AAAA", "AAA");
    QVERIFY( "mmmd" == alignment );
}

void Test::needlemanWunsch_intervals() {
    NeedlemanWunsch matcher;
    QByteArray seq;
    seq.append(-3);
    seq.append(4);
    seq.append(-5);
    QByteArray alignment = matcher.getAlignments(seq, seq);
    QVERIFY( "mmm" == alignment );
}

void Test::needlemanWunsch_transposition() {
    NeedlemanWunsch matcher;
    QByteArray alignment = matcher.getAlignments("ABC", "GHI", 6);
    QVERIFY( "mmm" == alignment );

    alignment = matcher.getAlignments("MURX", "AIFL", -12);
    QVERIFY( "mmmm" == alignment );
}

// GUIDOSERVICE

void Test::guidoService_gmnToScores_simple() {
    QString gmn = "[c d e f g]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 5 );
    QVERIFY( notes.at(0).pitch == 60 );
    QVERIFY( notes.at(1).pitch == 62 );
    QVERIFY( notes.at(2).pitch == 64 );
    QVERIFY( notes.at(3).pitch == 65 );
    QVERIFY( notes.at(4).pitch == 67 );
}

void Test::guidoService_gmnToScores_chord() {
    QString gmn = "[{c,e,g}]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 3 );
    QVERIFY( notes.at(0).pitch == 60 );
    QVERIFY( notes.at(1).pitch == 64 );
    QVERIFY( notes.at(2).pitch == 67 );
}

void Test::guidoService_gmnToScores_repeat() {
    QString gmn = "[c d \\beginRepeat e f g \\endRepeat a]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 9 );
}

void Test::guidoService_gmnToScores_finger() {
    QString gmn = "[c \\finger<\"3\"> e \\finger<\"1\"> g]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 3 );
    QVERIFY( notes.at(0).finger == UNKNOWN_FINGER );
    QVERIFY( notes.at(1).finger == MIDDLE );
    QVERIFY( notes.at(2).finger == THUMB );
}

void Test::guidoService_gmnToScores_voices() {
    QString gmn = "{[c e g],[d f g]}";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 6 );
}

void Test::guidoService_gmnToScores_duration_position() {
    QString gmn = "[c d/8 e/2 _/4 g/8]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 4 );
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
    QCOMPARE( ionisch.size(), 4 );
    QVERIFY( ionisch.at(0).pitch == 48 );
    QVERIFY( ionisch.at(1).pitch == 52 );
    QVERIFY( ionisch.at(2).pitch == 55 );
    QVERIFY( ionisch.at(3).pitch == 60 );

    QList<Score> dorisch = ScoreService::transposeStep(ionisch, 1);
    QCOMPARE( dorisch.size(), 4 );
    QVERIFY( dorisch.at(0).pitch == 50 );
    QVERIFY( dorisch.at(1).pitch == 53 );
    QVERIFY( dorisch.at(2).pitch == 57 );
    QVERIFY( dorisch.at(3).pitch == 62 );

    QList<Score> mixolydisch = ScoreService::transposeStep(ionisch, 4);
    QCOMPARE( mixolydisch.size(), 4 );
    QVERIFY( mixolydisch.at(0).pitch == 55 );
    QVERIFY( mixolydisch.at(1).pitch == 59 );
    QVERIFY( mixolydisch.at(2).pitch == 62 );
    QVERIFY( mixolydisch.at(3).pitch == 67 );

    QList<Score> aeolisch = ScoreService::transposeStep(ionisch, -2);
    QCOMPARE( aeolisch.size(), 4 );
    QVERIFY( aeolisch.at(0).pitch == 45 );
    QVERIFY( aeolisch.at(1).pitch == 48 );
    QVERIFY( aeolisch.at(2).pitch == 52 );
    QVERIFY( aeolisch.at(3).pitch == 57 );
}

void Test::scoreService_transposeSteps() {
    QString gmn = "[c e]";
    QList<Score> scores = GuidoService::gmnToScores(gmn);
    QVector<int> steps {0, 1, 2};

    QList<Score> steppedScores = ScoreService::transposeSteps(scores, steps);

    QCOMPARE( steppedScores.size(), 6 );
}

void Test::scoreService_addFingers() {
    QString gmn = "[c d e f g]";
    QList<Score> scores = GuidoService::gmnToScores(gmn);
    QVector<int> fingers {1, 2, 3};

    QList<Score> fingeredScores = ScoreService::addFingers(scores, fingers);

    QCOMPARE( fingeredScores.size(), 5 );
    QVERIFY( fingeredScores.at(0).finger == THUMB );
    QVERIFY( fingeredScores.at(1).finger == POINTER );
    QVERIFY( fingeredScores.at(2).finger == MIDDLE );
    QVERIFY( fingeredScores.at(3).finger == THUMB );
    QVERIFY( fingeredScores.at(4).finger == POINTER );
}

void Test::scoreService_concat() {
    QList<Score> empty1;
    QList<Score> empty2;

    QList<Score> empty3 = ScoreService::concat(empty1, empty2);
    QVERIFY( empty3.isEmpty() );


    QString gmn = "[c0/4 d e]";
    QList<Score> scores1 = GuidoService::gmnToScores(gmn);
    QList<Score> scores2 = GuidoService::gmnToScores(gmn);

    QList<Score> scores3 = ScoreService::concat(scores1, scores2);
    QCOMPARE( scores3.size(), 6 );

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

    QCOMPARE( merged.size(), 10);

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

void Test::scoreService_scoresToXYSequence() {
    QString gmn = "[c0 d _ f g]";
    QList<Score> scores = GuidoService::gmnToScores(gmn);
    QByteArray pitchSequence = ScoreService::scoresToPitchSequence(scores);
    QVERIFY( pitchSequence.size() == 4 );
    QVERIFY( pitchSequence.at(0) == 48 );
    QVERIFY( pitchSequence.at(1) == 50 );
    QVERIFY( pitchSequence.at(2) == 53 );
    QVERIFY( pitchSequence.at(3) == 55 );

    QByteArray intervalSequence = ScoreService::scoresToIntervalSequence(scores);
    QVERIFY( intervalSequence.size() == 3 );
    QVERIFY( intervalSequence.at(0) == 2 );
    QVERIFY( intervalSequence.at(1) == 3 );
    QVERIFY( intervalSequence.at(2) == 2 );
}

void Test::scoreService_filterFinger() {
    QString gmn = "[c0 d e f g a b c1]";
    QList<Score> scores = GuidoService::gmnToScores(gmn);

    QVector<int> fingers {5, 4, 3, 2, 1, 3, 2 };
    QList<Score> fingeredScores = ScoreService::addFingers(scores, fingers);

    QList<Score> filteredScores = ScoreService::filterFingers(fingeredScores, THUMB);
    QCOMPARE( filteredScores.size(), 1 );
    QVERIFY( filteredScores.at(0).pitch == 55 );

    filteredScores = ScoreService::filterFingers(fingeredScores, RING | MIDDLE);
    QCOMPARE( filteredScores.size(), 3 );
    QVERIFY( filteredScores.at(0).pitch == 50 );
    QVERIFY( filteredScores.at(1).pitch == 52 );
    QVERIFY( filteredScores.at(2).pitch == 57 );
}

void Test::scoreService_filterStatus() {
    QString gmn = "[c0 d e f g a b c1]";
    QList<Score> scores = GuidoService::gmnToScores(gmn);

    scores[0].status = MISSED;
    scores[1].status = PLAYED;
    scores[2].status = PLAYED;
    scores[3].status = EXTRA;
    scores[4].status = EXTRA;
    scores[5].status = OPEN;
    scores[6].status = OPEN;
    scores[7].status = OPEN;

    QList<Score> filteredScores = ScoreService::filterStatus(scores, PLAYED);
    QCOMPARE( filteredScores.size(), 2 );
    QVERIFY( filteredScores.at(0).pitch == 50 );
    QVERIFY( filteredScores.at(1).pitch == 52 );

    filteredScores = ScoreService::filterStatus(scores, EXTRA | MISSED);
    QCOMPARE( filteredScores.size(), 3 );
    QVERIFY( filteredScores.at(0).pitch == 48 );
    QVERIFY( filteredScores.at(1).pitch == 53 );
    QVERIFY( filteredScores.at(2).pitch == 55 );
}

// SONGFACTORIES

void Test::songFactories_hanon() {
    HanonSongFactory hanonSongFactory;

    QList<Song> songs = hanonSongFactory.getSongs();

    for (Song song : songs) {
        for (QList<Score> voice : song.voices.values()) {
            Fraction position(0);
            for (Score score : voice) {
                QVERIFY( score.position >= position );
                position = score.position;
            }
        }
    }
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

    QList<MidiPair> pairs;

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

    QCOMPARE( pairs.size(), 8 );

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

    QList<MidiPair> saveit;

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

    QList<MidiPair> loadit = MidiService::load(tempFile.fileName());

    QCOMPARE( loadit.size(), saveit.size() );
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

    QList<MidiPair> loadit;
    loadit = MidiService::load(fileName1);
    QCOMPARE( loadit.size(), 29*8 );

    loadit = MidiService::load(fileName2);
    QCOMPARE( loadit.size(), 29*8*2 );
}

// MATCHINGSERVICE

void Test::matchingService_midiPairs2xy() {
    NoteOnEvent A(  0, 0,   0, 0); NoteOffEvent a( 50, 0,   0, 0);
    NoteOnEvent B(100, 0, 127, 0); NoteOffEvent b(150, 0, 127, 0);
    NoteOnEvent C(200, 0,   0, 0); NoteOffEvent c(250, 0,   0, 0);
    NoteOnEvent D(200, 0,  64, 0); NoteOffEvent d(250, 0,  64, 0);

    MidiPair Aa(A, a);
    MidiPair Bb(B, b);
    MidiPair Cc(C);
    MidiPair Dd(D, d);

    QList<MidiPair> pairs;
    pairs.append(Aa);
    pairs.append(Bb);
    pairs.append(Cc);
    pairs.append(Dd);

    QByteArray pitchSequence = MatchingService::midiPairs2pitchSequence(pairs);
    QVERIFY( pitchSequence.size() == 4 );
    QVERIFY( pitchSequence.at(0) ==   0 );
    QVERIFY( pitchSequence.at(1) == 127 );
    QVERIFY( pitchSequence.at(2) ==   0 );
    QVERIFY( pitchSequence.at(3) ==  64 );

    QByteArray intervalSequence = MatchingService::midiPairs2intervalSequence(pairs);
    QVERIFY( intervalSequence.size() == 3 );
    QVERIFY( intervalSequence.at(0) == 127 );
    QVERIFY( intervalSequence.at(1) == -127 );
    QVERIFY( intervalSequence.at(2) == 64 );

    QByteArray pressedSequence = MatchingService::midiPairs2pressedSequence(pairs);
    QVERIFY( pressedSequence.size() == 4 );
    QVERIFY( pressedSequence.at(0) == MatchingService::RELEASED );
    QVERIFY( pressedSequence.at(1) == MatchingService::RELEASED );
    QVERIFY( pressedSequence.at(2) == MatchingService::PRESSED );
    QVERIFY( pressedSequence.at(3) == MatchingService::RELEASED );
}

void Test::matchingService_getSaveAlignment() {
    QVERIFY( MatchingService::getSaveAlignment("mid") == "" );
    QVERIFY( MatchingService::getSaveAlignment("immmmmmmmm") == "" );
    QVERIFY( MatchingService::getSaveAlignment("immmmmmmmmm") == "i" );
    QVERIFY( MatchingService::getSaveAlignment("dmmmmmmmmmm") == "d" );
    QVERIFY( MatchingService::getSaveAlignment("idmmmmmmmmmm") == "id" );
    QVERIFY( MatchingService::getSaveAlignment("idmmmmmmmmmmm") == "idm" );
    QVERIFY( MatchingService::getSaveAlignment("idmmmmmmmmmmmi") == "idm" );
}

void Test::matchingService_getAlignment_prunning() {
    QByteArray scorePitchSequence = "AbCdGhIjKlMnOpQrStUvWxYz";
    QByteArray midiPitchSequence = "AbdEfGhIjKlM";
    QByteArray oldAlignment = "mmdmiim";
    QByteArray expectedAlignment = "mmdmiimmmmmmmddddddddddddd";

    QByteArray normalAlignment = MatchingService::getAlingment(scorePitchSequence, midiPitchSequence);
    QByteArray prunnedAlignment = MatchingService::getAlingment(scorePitchSequence, midiPitchSequence, 0, oldAlignment);

    QVERIFY( normalAlignment == expectedAlignment );
    QVERIFY( prunnedAlignment == expectedAlignment );
}

void Test::matchingService_getTransposition() {
    QByteArray midiPitchSequence = "abcde";
    QByteArray scorePitchSequence = "ghijk";
    QByteArray intervalAlignment = "mmmm";
    int transposition = MatchingService::getTransposition(scorePitchSequence, midiPitchSequence, intervalAlignment);
    QCOMPARE( transposition, -6 );
}

void Test::matchingService_getQuality() {
    QVERIFY( MatchingService::getQuality("mmmmm", 0) == 1.0 );
    QVERIFY( MatchingService::getQuality("xxxxx", 0) == 0.0 );
}

void Test::matchingService_isFinished() {
    QVERIFY( !MatchingService::isFinished("mmmd", "....") );

    QVERIFY(  MatchingService::isFinished("mmmmmm", "......") );
    QVERIFY( !MatchingService::isFinished("mmmmmm", "...X..") );

    QVERIFY(  MatchingService::isFinished("mmmmi", ".....") );
    QVERIFY(  MatchingService::isFinished("mmmmi", "....X") );
    QVERIFY( !MatchingService::isFinished("mmmmi", "...X.") );

    QVERIFY(  MatchingService::isFinished("mimi", "....") );
    QVERIFY( !MatchingService::isFinished("mimi", ".X..") );
}

void Test::matchingService_cutMatchingMidiPairs() {
    NoteOnEvent A(  0, 0,   0, 0); NoteOffEvent a( 50, 0,   0, 0);
    NoteOnEvent B(100, 0, 127, 0); NoteOffEvent b(150, 0, 127, 0);
    NoteOnEvent C(200, 0,   0, 0); NoteOffEvent c(250, 0,   0, 0);

    MidiPair Aa(A, a);
    MidiPair Bb(B, b);
    MidiPair Cc(C, c);

    QList<MidiPair> pairs;
    pairs.append(Aa);
    pairs.append(Bb);
    pairs.append(Cc);

    QList<MidiPair> rest = MatchingService::cutMatchingMidiPairs(pairs, "mmi");
    QCOMPARE( pairs.size(), 2 );
    QCOMPARE( rest.size(), 1 );
    QVERIFY( rest.at(0) == Cc );
}

void Test::matchingService_merge() {
    QList<Score> scores;
    scores.append(Score(48));
    scores.append(Score(51));
    scores.append(Score(54));
    scores.append(Score(57));

    MidiPair A(NoteOnEvent(  0, 0, 48, 0));
    MidiPair Bb(NoteOnEvent(100, 0, 51, 0), NoteOffEvent(110, 0, 51, 0));
    MidiPair Cc(NoteOnEvent(100, 0, 57, 0), NoteOffEvent(110, 0, 57, 0));
    MidiPair Dd(NoteOnEvent(100, 0, 60, 0), NoteOffEvent(110, 0, 60, 0));

    QList<MidiPair> midiPairs;
    midiPairs.append(A);
    midiPairs.append(Bb);
    midiPairs.append(Cc);
    midiPairs.append(Dd);

    QByteArray pitchAlignment = "mmdmi";

    QList<Score> mergedScores = MatchingService::merge(scores, midiPairs, pitchAlignment);

    QCOMPARE( mergedScores.size(), 5 );
    QVERIFY( mergedScores.at(0).status == PLAYED );
    QVERIFY( mergedScores.at(0).midiPair == A );
    QVERIFY( mergedScores.at(1).status == PLAYED );
    QVERIFY( mergedScores.at(1).midiPair == Bb );
    QVERIFY( mergedScores.at(2).status == MISSED );
    QVERIFY( mergedScores.at(3).status == PLAYED );
    QVERIFY( mergedScores.at(3).midiPair == Cc );
    QVERIFY( mergedScores.at(4).status == EXTRA );
    QVERIFY( mergedScores.at(4).midiPair == Dd );
}

// MATCHINGHANDLER

void Test::matchingHandler_simple() {
    QString gmn = "[c0/16 d e f]";
    MatchingItem upItem = gmnToMatchingItem(gmn);

    gmn = "[g0/16 f e d]";
    MatchingItem downItem = gmnToMatchingItem(gmn);

    QList<MatchingItem> matchingItems;
    matchingItems.append(upItem);
    matchingItems.append(downItem);

    MatchingHandler matchingHandler(matchingItems);

    QSignalSpy positionSpy(&matchingHandler, SIGNAL(positionChanged(Fraction)));
    QSignalSpy songRecognizedSpy(&matchingHandler, SIGNAL(songRecognized(MatchingItem)));
    QSignalSpy songFinishedSpy(&matchingHandler, SIGNAL(songFinished(MatchingItem)));

    NoteOnEvent A(  0, 0, 48, 10); NoteOffEvent a( 10, 0, 48, 0);
    NoteOnEvent B(100, 0, 50, 10); NoteOffEvent b(110, 0, 50, 0);
    NoteOnEvent C(200, 0, 52, 10); NoteOffEvent c(210, 0, 52, 0);
    NoteOnEvent D(300, 0, 53, 10); NoteOffEvent d(310, 0, 53, 0);
    NoteOnEvent E(400, 0, 55, 10); NoteOffEvent e(410, 0, 55, 0);
    NoteOnEvent F(500, 0, 53, 10); NoteOffEvent f(510, 0, 53, 0);
    NoteOnEvent G(600, 0, 52, 10); NoteOffEvent g(610, 0, 52, 0);
    NoteOnEvent H(700, 0, 50, 10); NoteOffEvent h(710, 0, 50, 0);

    // Play accurate scores up
    matchingHandler.matchNoteOnEvent(A); matchingHandler.matchNoteOffEvent(a);
    matchingHandler.matchNoteOnEvent(B); matchingHandler.matchNoteOffEvent(b);
    matchingHandler.matchNoteOnEvent(C); matchingHandler.matchNoteOffEvent(c);
    matchingHandler.matchNoteOnEvent(D); matchingHandler.matchNoteOffEvent(d);

    QCOMPARE( songFinishedSpy.count(), 1 );

    QList<QVariant> arguments1 = songFinishedSpy.takeFirst();
    MatchingItem finishedItem1 = qvariant_cast<MatchingItem>(arguments1.at(0));

    QVERIFY( finishedItem1.song == upItem.song );
    QVERIFY( finishedItem1.pitchAlignment == "mmmm" );
    QVERIFY( finishedItem1.intervalAlignment == "mmm" );

    // Play accurate scores down
    matchingHandler.matchNoteOnEvent(E); matchingHandler.matchNoteOffEvent(e);
    matchingHandler.matchNoteOnEvent(F); matchingHandler.matchNoteOffEvent(f);
    matchingHandler.matchNoteOnEvent(G); matchingHandler.matchNoteOffEvent(g);
    matchingHandler.matchNoteOnEvent(H); matchingHandler.matchNoteOffEvent(h);

    QCOMPARE( songFinishedSpy.count(), 1 );
    QList<QVariant> arguments2 = songFinishedSpy.takeFirst();
    MatchingItem finishedItem2 = qvariant_cast<MatchingItem>(arguments2.at(0));

    QVERIFY( finishedItem2.song == downItem.song );
    QVERIFY( finishedItem2.pitchAlignment == "mmmm" );
    QVERIFY( finishedItem2.intervalAlignment == "mmm" );

    // And now we play not so accurate:
    // Play scores up, but lets start with the second song before we really finished the first one
    matchingHandler.matchNoteOnEvent(A); matchingHandler.matchNoteOffEvent(a);
    matchingHandler.matchNoteOnEvent(B); matchingHandler.matchNoteOffEvent(b);
    matchingHandler.matchNoteOnEvent(C); matchingHandler.matchNoteOffEvent(c);
    matchingHandler.matchNoteOnEvent(D); // ... wait with releasing ...

    matchingHandler.matchNoteOnEvent(E); // ... and start with the second song
    QCOMPARE( songFinishedSpy.count(), 0 );
    /* here comes the release    */ matchingHandler.matchNoteOffEvent(d);
    QCOMPARE( songFinishedSpy.count(), 1 );
    /* ... and here we continue  */ matchingHandler.matchNoteOffEvent(e);
    matchingHandler.matchNoteOnEvent(F); matchingHandler.matchNoteOffEvent(f);
    matchingHandler.matchNoteOnEvent(G); matchingHandler.matchNoteOffEvent(g);
    matchingHandler.matchNoteOnEvent(H); matchingHandler.matchNoteOffEvent(h);
    QCOMPARE( songFinishedSpy.count(), 2 );

    QList<QVariant> arguments3 = songFinishedSpy.takeFirst();
    MatchingItem finishedItem3 = qvariant_cast<MatchingItem>(arguments3.at(0));
    QList<QVariant> arguments4 = songFinishedSpy.takeFirst();
    MatchingItem finishedItem4 = qvariant_cast<MatchingItem>(arguments4.at(0));

    QVERIFY( *finishedItem1.midiPitchSequence == *finishedItem3.midiPitchSequence );
    QVERIFY( *finishedItem1.midiIntervalSequence == *finishedItem3.midiIntervalSequence );
    QVERIFY( *finishedItem1.pitchAlignment == *finishedItem3.pitchAlignment );
    QVERIFY( *finishedItem1.intervalAlignment == *finishedItem3.intervalAlignment );

    QVERIFY( *finishedItem2.midiPitchSequence == *finishedItem4.midiPitchSequence );
    QVERIFY( *finishedItem2.midiIntervalSequence == *finishedItem4.midiIntervalSequence );
    QVERIFY( *finishedItem2.pitchAlignment == *finishedItem4.pitchAlignment );
    QVERIFY( *finishedItem2.intervalAlignment == *finishedItem4.intervalAlignment );

    // Lets play one octave lower
    NoteOnEvent lA(  0, 0, 36, 10); NoteOffEvent la( 10, 0, 36, 0);
    NoteOnEvent lB(100, 0, 38, 10); NoteOffEvent lb(110, 0, 38, 0);
    NoteOnEvent lC(200, 0, 40, 10); NoteOffEvent lc(210, 0, 40, 0);
    NoteOnEvent lD(300, 0, 41, 10); NoteOffEvent ld(310, 0, 41, 0);

    matchingHandler.matchNoteOnEvent(lA); matchingHandler.matchNoteOffEvent(la);
    matchingHandler.matchNoteOnEvent(lB); matchingHandler.matchNoteOffEvent(lb);
    matchingHandler.matchNoteOnEvent(lC); matchingHandler.matchNoteOffEvent(lc);
    matchingHandler.matchNoteOnEvent(lD); matchingHandler.matchNoteOffEvent(ld);

    QCOMPARE( songFinishedSpy.count(), 1 );

    QList<QVariant> arguments5 = songFinishedSpy.takeFirst();
    MatchingItem finishedItem5 = qvariant_cast<MatchingItem>(arguments5.at(0));

    QVERIFY( finishedItem1.song == upItem.song );
    QCOMPARE( (int)finishedItem5.transposition, -12 );
    QVERIFY( finishedItem1.pitchAlignment == "mmmm" );
    QVERIFY( finishedItem1.intervalAlignment == "mmm" );
}

void Test::matchingHandler_hanonNo1Left() {
    QString fileName1("../../meisterschueler/test/midifiles/hanonNo1Left.mid");
    QFile hanonFile1(fileName1);
    QVERIFY( hanonFile1.exists() );

    QList<MidiPair> loadit;
    loadit = MidiService::load(fileName1);
    QCOMPARE( loadit.size(), 29*8 );

    QList<ChannelEvent> events;
    for (MidiPair p : loadit) {
        events.append(*p.noteOn);
        events.append(*p.noteOff);
    }

    qSort(events);

    SongService songService;

    MatchingHandler matchingHandler(songService.getMatchingItems());

    QSignalSpy positionSpy(&matchingHandler, SIGNAL(positionChanged(Fraction)));
    QSignalSpy songRecognizedSpy(&matchingHandler, SIGNAL(songRecognized(MatchingItem)));
    QSignalSpy songFinishedSpy(&matchingHandler, SIGNAL(songFinished(MatchingItem)));

    for (NoteEvent e : events) {
        if (e.type() == QEvent::User + STATUS_NOTEON) {
            //matchingHandler.noteOnEvent(static_cast<NoteOnEvent>(e));
            matchingHandler.matchNoteOnEvent(NoteOnEvent(e.getTime(), 0, e.getNote(), e.getVelocity()));

        } else if (e.type() == QEvent::User + STATUS_NOTEOFF) {
            //matchingHandler.noteOffEvent(static_cast<NoteOffEvent>(e));
            matchingHandler.matchNoteOffEvent(NoteOffEvent(e.getTime(), 0, e.getNote(), e.getVelocity()));
        }
    }

    QCOMPARE( songFinishedSpy.count(), 1 );

    QList<QVariant> arguments = songFinishedSpy.takeFirst();
    MatchingItem finishedItem = qvariant_cast<MatchingItem>(arguments.at(0));

    QVERIFY( finishedItem.song.name == "No. 1" );
    QVERIFY( finishedItem.pitchAlignment == QString("m").repeated(232) );
    QVERIFY( finishedItem.intervalAlignment.startsWith("mmmmmmmmmm") );
}

// STATISTICSSERVICE

void Test::statisticsService_statisticItem() {
    QVector<double> values {0.0, 1.0, 2.0, 3.0};
    StatisticItem item = StatisticsService::getStatisticItem(values);
    QCOMPARE( item.min, 0.0 );
    QCOMPARE( item.max, 3.0 );
    QCOMPARE( item.mean, 1.5 );
    QCOMPARE( item.variance, 1.25 );
    QCOMPARE( item.standarddeviation, sqrt(1.25) );

    QCOMPARE( item.spectrum.size(), 4 );
    QCOMPARE( item.spectrum.at(0), sqrt(36) );
    QCOMPARE( item.spectrum.at(1), sqrt(8) );
    QCOMPARE( item.spectrum.at(2), sqrt(4) );
    //QCOMPARE( item.spectrum.at(3), sqrt(8) );
}

void Test::statisticsService_statisticCluster() {
    QSKIP( "not yet implemented" );
}

// PLAYBACKHANDLER

void Test::playbackHandler_simple() {
    QList<MidiPair> pairs;
    pairs.append(MidiPair(NoteOnEvent(0, 0, 48, 10), NoteOffEvent(100, 0, 48, 0)));

    PlaybackHandler playbackHandler;
    QSignalSpy noteOnEventSpy(&playbackHandler, SIGNAL(gotNoteOnEvent(NoteOnEvent)));
    QSignalSpy noteOffEventSpy(&playbackHandler, SIGNAL(gotNoteOffEvent(NoteOffEvent)));

    playbackHandler.playMidiPairs(pairs);
    QCOMPARE( noteOnEventSpy.count(), 1 );
    QCOMPARE( noteOffEventSpy.count(), 0 );
    QTest::qSleep(105);
    QCOMPARE( noteOffEventSpy.count(), 1 );
}

// MIDIWRAPPER

void Test::midiWrapper_simple() {
    QSKIP( "Ensure jack is running: jackd -d alsa -X raw" );
    MidiWrapper midiWrapper;
    QStringList inputPorts = midiWrapper.getInputPorts();
    for (QString inputPort : inputPorts) {
        qDebug( "%s", inputPort.toStdString().c_str() );
    }
    QVERIFY( inputPorts.size() > 1 );
}

// HELPER FUNCTIONS

MatchingItem Test::gmnToMatchingItem(const QString& gmn) {
    QList<Score> scores = GuidoService::gmnToScores(gmn);
    QByteArray pitchSequence = ScoreService::scoresToPitchSequence(scores);
    QByteArray intervalSequence = ScoreService::scoresToIntervalSequence(scores);
    Song song;

    return MatchingItem(song, pitchSequence, intervalSequence);
}

QTEST_APPLESS_MAIN(Test)

#include "tst_test.moc"
