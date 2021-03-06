#include <QString>
#include <QtTest>

#include <QDebug>

#include "chordhandler.h"
#include "clusterhandler.h"
#include "guidoservice.h"
#include "hanonsongfactory.h"
#include "scoreservice.h"
#include "matchingservice.h"
#include "matchinghandler.h"
#include "merginghandler.h"
#include "midiservice.h"
#include "midiwrapper.h"
#include "needlemanwunsch.h"
#include "playbackhandler.h"
#include "recordhandler.h"
#include "resultmanager.h"
#include "score.h"
#include "songservice.h"
#include "statisticsservice.h"
#include "events.h"

class Test : public QObject
{
    Q_OBJECT

public:
    Test();

private:
    QString midiTestFiles;

private Q_SLOTS:
    void init();

    void channelEvent_comparisons();
    void noteOnEvent_comparisons();
    void midiPair_constructors();
    void midiPair_comparisons();
    void midiPairCluster_constructors();
    void midiPairCluster_comparisons();
    void score_comparisons();
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

    void guidoService_gmnToScores_notvalid();
    void guidoService_gmnToScores_simple();
    void guidoService_gmnToScores_chord();
    void guidoService_gmnToScores_repeat();
    void guidoService_gmnToScores_finger();
    void guidoService_gmnToScores_voices();
    void guidoService_gmnToScores_duration_position();
    void guidoService_getScore();

    void scoreService_transposeStep();
    void scoreService_transposeSteps();
    void scoreService_addFingers();
    void scoreService_concat();
    void scoreService_merge();
    void scoreService_scoresToXYSequence();
    void scoreService_filterFinger();
    void scoreService_filterStatus();

    void hanonSongFactory_simple();

    void songService_simple();
    void songService_findIntervalWithFingers();

    void midiService_addNote();
    void midiService_saveLoad();
    void midiService_loadHanon();

    void matchingService_midiPairs2xy();
    void matchingService_getSaveAlignment();
    void matchingService_getAlignment_prunning();
    void matchingService_getTransposition();
    void matchingService_getSongQuality();
    void matchingService_getChordQuality();
    void matchingService_isFinished();
    void matchingService_cutMatchingMidiPairs();
    void matchingService_merge();

    void matchingHandler_simple();
    void matchingHandler_hanonNo1Left();

    void chordHandler_cminor();

    void statisticsService_statisticItem();
    void statisticsService_statisticCluster();
    void statisticsService_statisticCluster_constantSpeed();
    void statisticsService_statisticCluster_variableSpeed();
    void statisticsService_statisticCluster_badValues();

    void mergingHandler_simple();

    void clusterHandler_simple();

    void playbackHandler_simple();

    void midiWrapper_simple();

private:
    MatchingItem gmnToMatchingItem(const QString &gmn);
};

Q_DECLARE_METATYPE(Fraction)

Test::Test()
{
    qRegisterMetaType<Fraction>("Fraction");
    qRegisterMetaType<MatchingItem>("MatchingItem");
    qRegisterMetaType<QList<ChannelEvent>>("QList<ChannelEvent>");
    qRegisterMetaType<NoteOnEvent>("NoteOnEvent");
    qRegisterMetaType<NoteOffEvent>("NoteOffEvent");
    qRegisterMetaType<MidiPairCluster>("MidiPairCluster");
    qRegisterMetaType<Score>("Score");
}

// INITIALIZATION

void Test::init() {
    midiTestFiles = QString("%1%2%3%4").arg(MAINPATH).arg(QDir::separator()).arg("meisterschueler-qt/test/midifiles").arg(QDir::separator());
}

// BASIC TYPES

void Test::channelEvent_comparisons() {
    ChannelEvent event(100, 0, 0, 0, Event::NoteOnEventType);
    ChannelEvent earlier(50, 0, 0, 0, Event::NoteOnEventType);
    ChannelEvent later(150, 0, 0, 0, Event::NoteOnEventType);

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
    QVERIFY( A == pair1.noteOn );
    QVERIFY( emptyNoteOffEvent == pair1.noteOff );

    NoteOffEvent a(10, 0, 10, 0);
    MidiPair pair2(A, a);
    QVERIFY( A == pair2.noteOn );
    QVERIFY( a == pair2.noteOff );
}

void Test::midiPair_comparisons() {
    MidiPair pair(NoteOnEvent(10, 0, 10, 64), NoteOffEvent(1000, 0, 10, 0));
    MidiPair same(NoteOnEvent(10, 0, 10, 64), NoteOffEvent(1000, 0, 10, 0));

    MidiPair earlier(NoteOnEvent(5, 0, 10, 0));
    MidiPair later(NoteOnEvent(15, 0, 10, 0));

    MidiPair higher(NoteOnEvent(10, 0, 20, 0));
    MidiPair lower(NoteOnEvent(10, 0, 5, 64));

    QVERIFY( pair == same );

    QVERIFY( pair != later );
    QVERIFY( pair < later );
    QCOMPARE( later < pair, false );

    QVERIFY( pair != earlier );
    QVERIFY( earlier < pair );
    QCOMPARE( pair < earlier, false );

    QVERIFY( pair != higher );
    QVERIFY( pair < higher );
    QCOMPARE( later < higher, false );

    QVERIFY( pair != lower );
    QVERIFY( lower < pair );
    QCOMPARE( pair < lower, false );

}

void Test::midiPairCluster_constructors() {
    MidiPair pair(NoteOnEvent(123, 0, 10, 64));
    MidiPairCluster mpc(pair);
    QVERIFY( mpc.time == 123 );
    QVERIFY( mpc.midiPairs.count() == 1 );
    QVERIFY( mpc.midiPairs.at(0) == pair );
}

void Test::midiPairCluster_comparisons() {
    MidiPairCluster mpc(MidiPair(NoteOnEvent(0, 0, 10, 64)));
    MidiPairCluster later(MidiPair(NoteOnEvent(1, 0, 10, 64)));
    MidiPairCluster other(MidiPair(NoteOnEvent(0, 0, 11, 64)));
    MidiPairCluster same;
    same.time = 0;
    same.midiPairs.append(MidiPair(NoteOnEvent(0, 0, 10, 64)));

    QVERIFY( mpc == same );
    QVERIFY( mpc != later );
    QVERIFY( mpc < later );
    QVERIFY( mpc != other );
}

void Test::score_comparisons() {
    Score a;
    Score b(0);

    QCOMPARE( a, b );
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

void Test::guidoService_gmnToScores_notvalid() {
    QList<Score> notes = GuidoService::gmnToScores("");
    QCOMPARE( notes.size(), 0 );
    notes = GuidoService::gmnToScores("MyFaultString");
    QCOMPARE( notes.size(), 0 );
}

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
    QSKIP("Broken in GUIDOLib");
    QString gmn = "[{c,e,g}]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 3 );
    QVERIFY( notes.at(0).pitch == 60 );
    QVERIFY( notes.at(1).pitch == 64 );
    QVERIFY( notes.at(2).pitch == 67 );
}

void Test::guidoService_gmnToScores_repeat() {
    QString gmn = "[c d \\repeatBegin e f g \\repeatEnd a]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);
    QCOMPARE( notes.size(), 9 );
}

void Test::guidoService_gmnToScores_finger() {
    QSKIP("Does not work with current GUIDOLib");
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

void Test::guidoService_getScore() {
    QString gmn = "[c/8 d e f g]";
    QList<Score> notes = GuidoService::gmnToScores(gmn);

    FloatRect box;

    GuidoDate a;
    a.num = 3;
    a.denom = 8;
    TimeSegment dates(a, a);

    GuidoElementInfos infos;
    infos.voiceNum = 1;

    RectInfos rectInfos(dates, infos);
    MapElement mapElement = std::pair<FloatRect, RectInfos>(box, rectInfos);

    Score score = GuidoService::getScore(notes, mapElement);
    QVERIFY( score.position == Fraction(3, 8) );
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
    QSKIP("Broken in GUIDOLib");
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

// HANONSONGFACTORY

void Test::hanonSongFactory_simple() {
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

// SONGSERVICE

void Test::songService_simple() {
    QString songDirectory = QString("%1%2%3").arg(MAINPATH).arg(QDir::separator()).arg("meisterschueler-misc");
    QList<Song> songs = SongService::getSongsFromDirectory(songDirectory);
    QVERIFY( songs.count() > 10 );
}

void Test::songService_findIntervalWithFingers() {
    HanonSongFactory hsf;
    Song song = hsf.getNo(0);
    int count = SongService::countSpecialInterval(song, 4, Finger::LITTLE, Finger::RING);
    QCOMPARE( count, 6 );
}

// MIDISERVICE

void Test::midiService_addNote() {
    NoteOnEvent A(  0, 0, 10, 0); NoteOffEvent a( 50, 0, 10, 0);
    NoteOnEvent B(100, 0, 20, 0); NoteOffEvent b(150, 0, 20, 0);
    NoteOnEvent C(120, 0, 30, 0); NoteOffEvent c(170, 0, 30, 0);
    NoteOnEvent D(130, 0, 25, 0); NoteOffEvent d(140, 0, 25, 0);
    NoteOnEvent E(131, 0, 12, 0); NoteOffEvent e(132, 0, 12, 0);
    NoteOnEvent F(500, 0, 70, 0); NoteOffEvent f(550, 0, 70, 0);

    QList<MidiPairCluster> mpc;

    MidiService::addNoteOn(mpc, A);
    MidiService::addNoteOff(mpc, a);
    MidiService::addNoteOn(mpc, B);
    MidiService::addNoteOff(mpc, b);
    MidiService::addNoteOn(mpc, C);
    MidiService::addNoteOn(mpc, D);
    MidiService::addNoteOn(mpc, E);
    MidiService::addNoteOff(mpc, e);
    MidiService::addNoteOff(mpc, d);
    MidiService::addNoteOff(mpc, c);
    MidiService::addNoteOn(mpc, F);
    MidiService::addNoteOff(mpc, f);

    QCOMPARE( mpc.size(), 3 );
    QVERIFY( mpc.at(0).midiPairs.count() == 1 );
    QVERIFY( A == mpc.at(0).midiPairs.at(0).noteOn );
    QVERIFY( a == mpc.at(0).midiPairs.at(0).noteOff );

    QVERIFY( mpc.at(1).midiPairs.count() == 4 );
    QVERIFY( B == mpc.at(1).midiPairs.at(1).noteOn );
    QVERIFY( b == mpc.at(1).midiPairs.at(1).noteOff );
    QVERIFY( C == mpc.at(1).midiPairs.at(3).noteOn );
    QVERIFY( c == mpc.at(1).midiPairs.at(3).noteOff );
    QVERIFY( D == mpc.at(1).midiPairs.at(2).noteOn );
    QVERIFY( d == mpc.at(1).midiPairs.at(2).noteOff );
    QVERIFY( E == mpc.at(1).midiPairs.at(0).noteOn );
    QVERIFY( e == mpc.at(1).midiPairs.at(0).noteOff );

    QVERIFY( mpc.at(2).midiPairs.count() == 1 );
    QVERIFY( F == mpc.at(2).midiPairs.at(0).noteOn );
    QVERIFY( f == mpc.at(2).midiPairs.at(0).noteOff );
}

void Test::midiService_saveLoad() {
    NoteOnEvent A(  0, 0, 48, 0); NoteOffEvent a( 50, 0, 48, 0);
    NoteOnEvent B(100, 0, 52, 0); NoteOffEvent b(150, 0, 52, 0);
    NoteOnEvent C(200, 0, 55, 0); NoteOffEvent c(250, 0, 55, 0);

    QList<ChannelEvent> saveit;

    saveit.append(A);
    saveit.append(a);
    saveit.append(B);
    saveit.append(b);
    saveit.append(C);
    saveit.append(c);

    QTemporaryFile tempFile("midiService");
    tempFile.open();
    tempFile.close();

    bool success = MidiService::save(tempFile.fileName(), saveit);
    QCOMPARE( success, true );

    QList<ChannelEvent> loadit = MidiService::load(tempFile.fileName());

    QCOMPARE( loadit.size(), saveit.size() );
    for (int i = 0; i < loadit.size(); ++i) {
        QVERIFY( loadit.at(i) == saveit.at(i) );
    }
}

void Test::midiService_loadHanon() {
    QString fileName1(midiTestFiles + "hanonNo1Left.mid");
    QString fileName2(midiTestFiles + "hanonNo1Both.mid");
    QFile hanonFile1(fileName1);
    QFile hanonFile2(fileName2);
    QVERIFY( hanonFile1.exists() );
    QVERIFY( hanonFile2.exists() );

    QList<ChannelEvent> loadit;
    loadit = MidiService::load(fileName1);
    QCOMPARE( loadit.size(), 2*29*8 );

    loadit = MidiService::load(fileName2);
    QCOMPARE( loadit.size(), 2*29*8*2 );
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

    QList<MidiPairCluster> pairs;
    pairs.append(Aa);   // TODO: hier wird implizit der MidiPairCluster-Konstruktor genommen... will man das?
    pairs.append(Bb);
    pairs.append(Cc);
    pairs.append(Dd);

    QByteArray pitchSequence = MatchingService::midiPairClusters2pitchSequence(pairs);
    QVERIFY( pitchSequence.size() == 4 );
    QVERIFY( pitchSequence.at(0) ==   0 );
    QVERIFY( pitchSequence.at(1) == 127 );
    QVERIFY( pitchSequence.at(2) ==   0 );
    QVERIFY( pitchSequence.at(3) ==  64 );

    QByteArray intervalSequence = MatchingService::midiPairClusters2intervalSequence(pairs);
    QVERIFY( intervalSequence.size() == 3 );
    QVERIFY( intervalSequence.at(0) == 127 );
    QVERIFY( intervalSequence.at(1) == -127 );
    QVERIFY( intervalSequence.at(2) == 64 );

    QByteArray pressedSequence = MatchingService::midiPairClusters2pressedSequence(pairs);
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

void Test::matchingService_getSongQuality() {
    QVERIFY( MatchingService::getSongQuality("mmmmm", 0) == 1.0 );
    QVERIFY( MatchingService::getSongQuality("xxxxx", 0) == 0.0 );
}

void Test::matchingService_getChordQuality() {
    QVERIFY( MatchingService::getChordQuality("mmm") == 1.0 );
    QVERIFY( MatchingService::getChordQuality("xxx") == 0.0 );
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

    QList<MidiPairCluster> pairs;
    pairs.append(Aa);
    pairs.append(Bb);
    pairs.append(Cc);

    QList<MidiPairCluster> rest = MatchingService::cutMatchingMidiPairs(pairs, "mmi");
    QCOMPARE( pairs.size(), 2 );
    QCOMPARE( rest.size(), 1 );
    QCOMPARE( rest.at(0).midiPairs.size(), 1 );
    QVERIFY( rest.at(0).midiPairs.at(0) == Cc );
}

void Test::matchingService_merge() {
    QList<Score> scores;
    scores.append(Score(48));
    scores.append(Score(51));
    scores.append(Score(54));
    scores.append(Score(57));
    scores.append(Score(63));
    scores.append(Score(66));

    MidiPair A(NoteOnEvent(  0, 0, 48, 0));
    MidiPair Bb(NoteOnEvent(100, 0, 51, 0), NoteOffEvent(110, 0, 51, 0));
    MidiPair Cc(NoteOnEvent(100, 0, 57, 0), NoteOffEvent(110, 0, 57, 0));
    MidiPair Dd(NoteOnEvent(100, 0, 60, 0), NoteOffEvent(110, 0, 60, 0));
    MidiPair Ee(NoteOnEvent(100, 0, 63, 0), NoteOffEvent(110, 0, 63, 0));
    MidiPair Ff(NoteOnEvent(100, 0, 65, 0), NoteOffEvent(110, 0, 65, 0));

    QList<MidiPairCluster> mpc;
    mpc.append(A);
    mpc.append(Bb);
    mpc.append(Cc);
    mpc.append(Dd);
    mpc.append(Ee);
    mpc.append(Ff);

    QByteArray pitchAlignment = "mmdmimw";

    QList<Score> mergedScores = MatchingService::merge(scores, mpc, pitchAlignment);

    QCOMPARE( mergedScores.size(), 7 );
    QVERIFY( mergedScores.at(0).status == PLAYED );
    QVERIFY( mergedScores.at(0).midiPair == A );
    QVERIFY( mergedScores.at(1).status == PLAYED );
    QVERIFY( mergedScores.at(1).midiPair == Bb );
    QVERIFY( mergedScores.at(2).status == MISSED );
    QVERIFY( mergedScores.at(3).status == PLAYED );
    QVERIFY( mergedScores.at(3).midiPair == Cc );
    QVERIFY( mergedScores.at(4).status == EXTRA );
    QVERIFY( mergedScores.at(4).midiPair == Dd );
    QVERIFY( mergedScores.at(5).status == PLAYED );
    QVERIFY( mergedScores.at(5).midiPair == Ee );
    QVERIFY( mergedScores.at(6).status == FAILED );
    QVERIFY( mergedScores.at(6).midiPair == Ff );
}

// MATCHINGHANDLER

void Test::matchingHandler_simple() {
    QString gmnUp = "[c0/16 d e f]";
    MatchingItem upItem = gmnToMatchingItem(gmnUp);

    QString gmnDown = "[g0/16 f e d]";
    MatchingItem downItem = gmnToMatchingItem(gmnDown);

    QList<MatchingItem> matchingItems;
    matchingItems.append(upItem);
    matchingItems.append(downItem);

    MatchingHandler matchingHandler(matchingItems);

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
    QCOMPARE( songRecognizedSpy.count(), 2 );
    matchingHandler.matchNoteOnEvent(B); matchingHandler.matchNoteOffEvent(b);
    QCOMPARE( songRecognizedSpy.count(), 4 );
    matchingHandler.matchNoteOnEvent(C); matchingHandler.matchNoteOffEvent(c);
    QCOMPARE( songRecognizedSpy.count(), 6 );
    matchingHandler.matchNoteOnEvent(D); matchingHandler.matchNoteOffEvent(d);
    QCOMPARE( songRecognizedSpy.count(), 8 );

    QCOMPARE( songFinishedSpy.count(), 1 );
    QList<QVariant> arguments1 = songFinishedSpy.takeFirst();
    MatchingItem finishedItem1 = qvariant_cast<MatchingItem>(arguments1.at(0));

    QVERIFY( finishedItem1.song == upItem.song );
    QVERIFY( finishedItem1.pitchAlignment == "mmmm" );
    QVERIFY( finishedItem1.intervalAlignment == "mmm" );
    QCOMPARE( finishedItem1.mergedScores.count(), 4 );

    // Play accurate scores down
    matchingHandler.matchNoteOnEvent(E); matchingHandler.matchNoteOffEvent(e);
    QCOMPARE( songRecognizedSpy.count(), 10 );
    matchingHandler.matchNoteOnEvent(F); matchingHandler.matchNoteOffEvent(f);
    QCOMPARE( songRecognizedSpy.count(), 12 );
    matchingHandler.matchNoteOnEvent(G); matchingHandler.matchNoteOffEvent(g);
    QCOMPARE( songRecognizedSpy.count(), 14 );
    matchingHandler.matchNoteOnEvent(H); matchingHandler.matchNoteOffEvent(h);
    QCOMPARE( songRecognizedSpy.count(), 16 );

    QCOMPARE( songFinishedSpy.count(), 1 );
    QList<QVariant> arguments2 = songFinishedSpy.takeFirst();
    MatchingItem finishedItem2 = qvariant_cast<MatchingItem>(arguments2.at(0));

    QVERIFY( finishedItem2.song == downItem.song );
    QVERIFY( finishedItem2.pitchAlignment == "mmmm" );
    QVERIFY( finishedItem2.intervalAlignment == "mmm" );
    QCOMPARE( finishedItem2.mergedScores.count(), 4 );

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
    QString fileName1(midiTestFiles + "hanonNo1Left.mid");
    QFile hanonFile1(fileName1);
    QVERIFY( hanonFile1.exists() );

    QList<ChannelEvent> loadit;
    loadit = MidiService::load(fileName1);
    QCOMPARE( loadit.size(), 2*29*8 );

    QList<Song> songs = SongService::getSongsBuiltIn();
    QList<MatchingItem> matchingItems = SongService::createMatchingItems(songs);
    MatchingHandler matchingHandler(matchingItems);

    QSignalSpy songRecognizedSpy(&matchingHandler, SIGNAL(songRecognized(MatchingItem)));
    QSignalSpy songFinishedSpy(&matchingHandler, SIGNAL(songFinished(MatchingItem)));

    for (ChannelEvent e : loadit) {
        if (e.type() == Event::NoteOnEventType) {
            matchingHandler.matchNoteOnEvent(static_cast<NoteOnEvent>(e));
        } else if (e.type() == Event::NoteOffEventType) {
            matchingHandler.matchNoteOffEvent(static_cast<NoteOffEvent>(e));
        }
    }

    QCOMPARE( songFinishedSpy.count(), 1 );

    QList<QVariant> arguments = songFinishedSpy.takeFirst();
    MatchingItem finishedItem = qvariant_cast<MatchingItem>(arguments.at(0));

    QVERIFY( finishedItem.song.name == "No. 1" );
    QVERIFY( finishedItem.pitchAlignment == QString("m").repeated(232) );
    QVERIFY( finishedItem.intervalAlignment.startsWith("mmmmmmmmmm") );
}

// CHORDHANDLER

void Test::chordHandler_cminor() {
    ChordHandler chordHandler;
    QSignalSpy chordRecognizedSpy(&chordHandler, SIGNAL(chordRecognized(MatchingItem)));

    chordHandler.matchNoteOnEvent(NoteOnEvent(0, 0, 48, 12));
    chordHandler.matchNoteOnEvent(NoteOnEvent(0, 0, 51, 12));
    chordHandler.matchNoteOnEvent(NoteOnEvent(0, 0, 55, 12));

    QCOMPARE( chordRecognizedSpy.count(), 1 );

    QList<QVariant> arguments = chordRecognizedSpy.takeFirst();
    MatchingItem recognizedItem = qvariant_cast<MatchingItem>(arguments.at(0));

    QVERIFY( recognizedItem.song.name == "Cm" );
}

// STATISTICSSERVICE

void Test::statisticsService_statisticItem() {
    QVector<double> noValues;
    StatisticItem item = StatisticsService::getStatisticItem(noValues);
    QVERIFY( item.values == noValues );

    QVector<double> values {0.0, 1.0, 2.0, 3.0};
    item = StatisticsService::getStatisticItem(values);
    QVERIFY( item.values == values );
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
    QList<MidiPairCluster> mpc;
    MidiPairCluster a(NoteOnEvent(   0, 0, 60, 50));
    MidiPairCluster b(NoteOnEvent(1000, 0, 62, 50));
    b.midiPairs.append(MidiPair(NoteOnEvent(1010, 0, 64, 50)));
    b.midiPairs.append(MidiPair(NoteOnEvent(1020, 0, 65, 50)));

    MidiPairCluster c(NoteOnEvent(1800, 0, 65, 50));
    c.midiPairs.append(MidiPair(NoteOnEvent(1830, 0, 67, 50)));

    MidiPairCluster d(NoteOnEvent(2400, 0, 67, 50));

    mpc.append(a);
    StatisticCluster statisticCluster = StatisticsService::getStatisticCluster(mpc);
    QCOMPARE( statisticCluster.speed.values.size(), 1 );
    QCOMPARE( statisticCluster.speed.values.at(0), 60.0 );

    mpc.append(b);
    statisticCluster = StatisticsService::getStatisticCluster(mpc);
    QCOMPARE( statisticCluster.speed.values.size(), 2 );
    QCOMPARE( statisticCluster.speed.values.at(0), 60.0 );
    QCOMPARE( statisticCluster.speed.values.at(1), 60.0 );

    mpc.append(c);
    statisticCluster = StatisticsService::getStatisticCluster(mpc);
    QCOMPARE( statisticCluster.speed.values.size(), 3 );
    QCOMPARE( statisticCluster.speed.values.at(0), 60.0 );
    QCOMPARE( statisticCluster.speed.values.at(1), 75.0 );
    QCOMPARE( statisticCluster.speed.values.at(2), 75.0 );

    mpc.append(d);
    statisticCluster = StatisticsService::getStatisticCluster(mpc);
    QCOMPARE( statisticCluster.speed.values.size(), 4 );
    QCOMPARE( statisticCluster.speed.values.at(0),  60.0 );
    QCOMPARE( statisticCluster.speed.values.at(1),  75.0 );
    QCOMPARE( statisticCluster.speed.values.at(2), 100.0 );
    QCOMPARE( statisticCluster.speed.values.at(3), 100.0 );

    statisticCluster = StatisticsService::getStatisticCluster(mpc, Frac_1_2);
    QCOMPARE( statisticCluster.speed.values.size(), 4 );
    QCOMPARE( statisticCluster.speed.values.at(0), 120.0 );
    QCOMPARE( statisticCluster.speed.values.at(1), 150.0 );
    QCOMPARE( statisticCluster.speed.values.at(2), 200.0 );
    QCOMPARE( statisticCluster.speed.values.at(3), 200.0 );

    QCOMPARE( statisticCluster.offset.values.size(), 7 );
    QCOMPARE( statisticCluster.offset.values.at(0),  0.0 );
    QCOMPARE( statisticCluster.offset.values.at(1),  0.0 );
    QCOMPARE( statisticCluster.offset.values.at(2), 10.0 );
    QCOMPARE( statisticCluster.offset.values.at(3), 20.0 );
    QCOMPARE( statisticCluster.offset.values.at(4),  0.0 );
    QCOMPARE( statisticCluster.offset.values.at(5), 30.0 );
    QCOMPARE( statisticCluster.offset.values.at(6),  0.0 );
}

void Test::statisticsService_statisticCluster_constantSpeed() {
    QList<Score> scores;
    Score a(48, Status::PLAYED);
    a.duration = Fraction(1, 4);
    a.midiPair = MidiPair(NoteOnEvent(  0, 0, 48, 0), NoteOffEvent(200, 0, 48, 0));
    scores.append(a);

    Score b(48, Status::PLAYED);
    b.duration = Fraction(1, 4);
    b.midiPair = MidiPair(NoteOnEvent(200, 0, 48, 0), NoteOffEvent(400, 0, 48, 0));
    scores.append(b);

    Score c(48, Status::PLAYED);
    c.duration = Fraction(1, 4);
    c.midiPair = MidiPair(NoteOnEvent(400, 0, 48, 0), NoteOffEvent(600, 0, 48, 0));
    scores.append(c);

    Score d(48, Status::PLAYED);
    d.duration = Fraction(1, 4);
    d.midiPair = MidiPair(NoteOnEvent(600, 0, 48, 0), NoteOffEvent(800, 0, 48, 0));
    scores.append(d);

    StatisticCluster statisticCluster = StatisticsService::getStatisticCluster(scores);
    QCOMPARE( statisticCluster.speed.min, 300.0 );
    QCOMPARE( statisticCluster.speed.mean, 300.0 );
    QCOMPARE( statisticCluster.speed.max, 300.0 );

    Score e(48, Status::MISSED);
    e.duration = Fraction(1, 4);
    scores.append(e);

    Score f(48, Status::PLAYED);
    f.duration = Fraction(1, 4);
    f.midiPair = MidiPair(NoteOnEvent(1000, 0, 48, 0), NoteOffEvent(1200, 0, 48, 0));
    scores.append(f);

    statisticCluster = StatisticsService::getStatisticCluster(scores);
    QCOMPARE( statisticCluster.speed.min, 300.0 );
    QCOMPARE( statisticCluster.speed.mean, 300.0 );
    QCOMPARE( statisticCluster.speed.max, 300.0 );

    Score g(48, Status::EXTRA);
    g.midiPair = MidiPair(NoteOnEvent(1150, 0, 48, 0), NoteOffEvent(1160, 0, 48, 0));
    scores.append(g);

    Score h(48, Status::PLAYED);
    h.duration = Fraction(1, 4);
    h.midiPair = MidiPair(NoteOnEvent(1200, 0, 48, 0), NoteOffEvent(1400, 0, 48, 0));
    scores.append(h);

    statisticCluster = StatisticsService::getStatisticCluster(scores);
    QCOMPARE( statisticCluster.speed.min, 300.0 );
    QCOMPARE( statisticCluster.speed.mean, 300.0 );
    QCOMPARE( statisticCluster.speed.max, 300.0 );

    Score i(48, Status::OPEN);
    i.duration = Fraction(1, 4);
    scores.append(i);

    statisticCluster = StatisticsService::getStatisticCluster(scores);
    QCOMPARE( statisticCluster.speed.min, 300.0 );
    QCOMPARE( statisticCluster.speed.mean, 300.0 );
    QCOMPARE( statisticCluster.speed.max, 300.0 );
}

void Test::statisticsService_statisticCluster_variableSpeed() {
    QSKIP("Not yet implemented");
}

// TODO: standard values? 0.0? 60.0?
void Test::statisticsService_statisticCluster_badValues() {
    QList<Score> scores;
    scores.append(Score(48, Status::MISSED));
    scores.append(Score(50, Status::MISSED));
    scores.append(Score(52, Status::MISSED));
    StatisticCluster statisticCluster = StatisticsService::getStatisticCluster(scores);
    //QCOMPARE( statisticCluster.speed.min, 0.0 );
    //QCOMPARE( statisticCluster.speed.mean, 0.0 );
    //QCOMPARE( statisticCluster.speed.max, 0.0 );
}

// MERGINGHANDLER

void Test::mergingHandler_simple() {
    MergingHandler mergingHandler;
    QSignalSpy scoreChangedSpy(&mergingHandler, SIGNAL(scoreChanged(Score, Score)));
    QSignalSpy scoreInsertedSpy(&mergingHandler, SIGNAL(scoreInserted(Score)));
    QSignalSpy scoreDeletedSpy(&mergingHandler, SIGNAL(scoreDeleted(Score)));
    QSignalSpy positionChangedSpy(&mergingHandler, SIGNAL(positionChanged(Fraction)));

    Score A(48, PLAYED);
    A.position = Fraction(1, 4);

    Score B(50, PLAYED);
    B.position = Fraction(2, 4);

    Score C(52, PLAYED);
    C.position = Fraction(3, 4);

    Score c(52, OPEN);
    c.position = Fraction(3, 4);

    Score d(53, OPEN);
    d.position = Fraction(4, 4);

    Score dx(53, MISSED);
    dx.position = Fraction(0, 4);

    Score E_pressed(55, PLAYED);
    E_pressed.position = Fraction(5, 4);
    E_pressed.midiPair.noteOn = NoteOnEvent(0, 0, 55, 20);

    Score E_released(55, PLAYED);
    E_released.position = Fraction(5, 4);
    E_released.midiPair.noteOn = NoteOnEvent(0, 0, 55, 20);
    E_released.midiPair.noteOff = NoteOffEvent(0, 0, 55, 0);

    Score e(55, OPEN);
    e.position = Fraction(5, 4);

    MatchingItem itemBefore;
    itemBefore.mergedScores = QList<Score>() << A << B << c << d << e;
    itemBefore.pitchAlignment = "mmooo";

    MatchingItem itemMiddle;
    itemMiddle.mergedScores = QList<Score>() << A << B << C << d << e;
    itemMiddle.pitchAlignment = "mmmoo";

    MatchingItem itemLast;
    itemLast.mergedScores = QList<Score>() << A << B << C << dx << E_pressed;
    itemLast.pitchAlignment = "mmmdm";

    MatchingItem itemVeryLast;
    itemVeryLast.mergedScores = QList<Score>() << A << B << C << dx << E_released;
    itemVeryLast.pitchAlignment = "mmmdm";

    MatchingItem itemVeryVeryLast;
    itemVeryVeryLast.mergedScores = QList<Score>() << A << B << C << dx << e;
    itemVeryVeryLast.pitchAlignment = "mmmdd";

    // itemBefore
    mergingHandler.eatMatchingItem(itemBefore);

    QCOMPARE( scoreInsertedSpy.count(), 2 );
    QList<QVariant> arguments = scoreInsertedSpy.takeFirst();
    Score insertedScore1 = qvariant_cast<Score>(arguments.at(0));
    QCOMPARE( insertedScore1, A );
    arguments = scoreInsertedSpy.takeFirst();
    Score insertedScore2 = qvariant_cast<Score>(arguments.at(0));
    QCOMPARE( insertedScore2, B );

    QCOMPARE( positionChangedSpy.count(), 1 );
    arguments = positionChangedSpy.takeFirst();
    Fraction positionBefore = qvariant_cast<Fraction>(arguments.at(0));
    QVERIFY( positionBefore == Fraction(2,4) );

    // itemMiddle
    mergingHandler.eatMatchingItem(itemMiddle);

    QCOMPARE( scoreInsertedSpy.count(), 1 );
    arguments = scoreInsertedSpy.takeFirst();
    Score insertedScore3 = qvariant_cast<Score>(arguments.at(0));
    QCOMPARE( insertedScore3, C );

    QCOMPARE( positionChangedSpy.count(), 1 );
    arguments = positionChangedSpy.takeFirst();
    Fraction positionAfter = qvariant_cast<Fraction>(arguments.at(0));
    QVERIFY( positionAfter == Fraction(3,4) );

    // itemLast
    mergingHandler.eatMatchingItem(itemLast);

    QCOMPARE( scoreInsertedSpy.count(), 2 );
    arguments = scoreInsertedSpy.takeFirst();
    Score insertedScore4 = qvariant_cast<Score>(arguments.at(0));
    QCOMPARE( insertedScore4, dx );
    arguments = scoreInsertedSpy.takeFirst();
    Score insertedScore5 = qvariant_cast<Score>(arguments.at(0));
    QCOMPARE( insertedScore5, E_pressed );

    QCOMPARE( positionChangedSpy.count(), 1 );
    arguments = positionChangedSpy.takeFirst();
    Fraction positionLast = qvariant_cast<Fraction>(arguments.at(0));
    QVERIFY( positionLast == Fraction(5,4) );

    // itemVeryLast
    mergingHandler.eatMatchingItem(itemVeryLast);

    QCOMPARE( scoreChangedSpy.count(), 1 );
    arguments = scoreChangedSpy.takeFirst();
    Score changedScoreBefore = qvariant_cast<Score>(arguments.at(0));
    Score changedScoreAfter = qvariant_cast<Score>(arguments.at(1));
    QCOMPARE( changedScoreBefore, E_pressed );
    QCOMPARE( changedScoreAfter, E_released );

    // itemVeryVeryLast
    mergingHandler.eatMatchingItem(itemVeryVeryLast);

    QCOMPARE( scoreDeletedSpy.count(), 1 );
    arguments = scoreDeletedSpy.takeFirst();
    Score deletedScore = qvariant_cast<Score>(arguments.at(0));
    QCOMPARE( deletedScore, E_released );

    QCOMPARE( positionChangedSpy.count(), 1 );
    arguments = positionChangedSpy.takeFirst();
    Fraction positionVeryLast = qvariant_cast<Fraction>(arguments.at(0));
    QVERIFY( positionVeryLast == Fraction(3,4) );
}

// CLUSTERHANDLER

void Test::clusterHandler_simple() {
    ClusterHandler clusterHandler;
    QSignalSpy midiPairClusterSpy(&clusterHandler, SIGNAL(gotMidiPairCluster(MidiPairCluster)));

    clusterHandler.matchNoteOnEvent(NoteOnEvent(  0, 0, 48, 10));
    clusterHandler.matchNoteOffEvent(NoteOffEvent(100, 0, 48, 0));
    QCOMPARE( midiPairClusterSpy.count(), 0 );
    QTest::qSleep(50);
    QCOMPARE( midiPairClusterSpy.count(), 1 );
}

// PLAYBACKHANDLER

void Test::playbackHandler_simple() {
    QList<MidiPair> pairs;
    pairs.append(MidiPair(NoteOnEvent(  0, 0, 48, 10), NoteOffEvent(100, 0, 48, 0)));
    pairs.append(MidiPair(NoteOnEvent(200, 0, 50, 10), NoteOffEvent(300, 0, 50, 0)));

    PlaybackHandler playbackHandler;
    QSignalSpy noteOnEventSpy(&playbackHandler, SIGNAL(gotNoteOnEvent(NoteOnEvent)));
    QSignalSpy noteOffEventSpy(&playbackHandler, SIGNAL(gotNoteOffEvent(NoteOffEvent)));

    playbackHandler.setMidiPairs(pairs);
    playbackHandler.play();
    QTest::qSleep(10);
    QCOMPARE( noteOnEventSpy.count(), 1 );
    QCOMPARE( noteOffEventSpy.count(), 0 );
    QTest::qSleep(105);
    QCOMPARE( noteOffEventSpy.count(), 1 );
    playbackHandler.stop();
    QTest::qSleep(1000);
    QCOMPARE( noteOnEventSpy.count(), 1 );
    QCOMPARE( noteOffEventSpy.count(), 1 );
    playbackHandler.play();
    QTest::qSleep(300);
    QCOMPARE( noteOnEventSpy.count(), 2 );
    QCOMPARE( noteOffEventSpy.count(), 2 );
}

// MIDIWRAPPER

void Test::midiWrapper_simple() {
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
    song.name = gmn;
    song.voices.insert(Hand::LEFT, scores);

    return MatchingItem(song, pitchSequence, intervalSequence);
}

//QTEST_APPLESS_MAIN(Test)
QTEST_MAIN(Test)

#include "tst_test.moc"
