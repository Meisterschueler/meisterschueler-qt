#include "matchinghandler.h"

#include <QDebug>

#include "matchingservice.h"
#include "midiservice.h"
#include "needlemanwunsch.h"

MatchingHandler::MatchingHandler(const QList<MatchingItem> &matchingItems) : matchingItems(matchingItems)
{
    midiPairClusters = QSharedPointer<QList<MidiPairCluster>>(new QList<MidiPairCluster>());
    oldPitchSequence = QSharedPointer<QByteArray>(new QByteArray());
}

void MatchingHandler::init() {
    for (QList<MatchingItem>::iterator i = matchingItems.begin(); i != matchingItems.end(); ++i) {
        (*i).enabled = true;
        (*i).pitchAlignment = "";
        (*i).intervalAlignment = "";
    }
}

void MatchingHandler::reset() {
    init();
    (*midiPairClusters).clear();
    (*oldPitchSequence).clear();
}

void MatchingHandler::matchNoteOnEvent(NoteOnEvent noteOn) {
    MidiService::addNoteOn(*midiPairClusters, noteOn);
    match();
}

void MatchingHandler::matchNoteOffEvent(NoteOffEvent noteOff) {
    MidiService::addNoteOff(*midiPairClusters, noteOff);
    match();
}

void MatchingHandler::matchMidiPairClusters(QList<MidiPairCluster> mpc) {
    (*midiPairClusters) = mpc;
    match();
}

void MatchingHandler::match() {
    QSharedPointer<QByteArray> midiPitchSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiPairClusters2pitchSequence(*midiPairClusters)));
    QSharedPointer<QByteArray> midiIntervalSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiPairClusters2intervalSequence(*midiPairClusters)));
    QSharedPointer<QByteArray> pressedSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiPairClusters2pressedSequence(*midiPairClusters)));

    bool pitchSequenceChanged = *oldPitchSequence != *midiPitchSequence;

    for (QList<MatchingItem>::iterator i = matchingItems.begin(); i != matchingItems.end(); ++i) {
        MatchingItem item = *i;
        if (!item.enabled) {
            continue;
        }

        item.midiPairClusters = midiPairClusters;
        item.midiPitchSequence = midiPitchSequence;
        item.midiIntervalSequence = midiIntervalSequence;
        item.pressedSequence = pressedSequence;

        if (pitchSequenceChanged) {
            QByteArray saveAlignment = MatchingService::getSaveAlignment(item.pitchAlignment);

            if (saveAlignment.isEmpty() || (*oldPitchSequence).isEmpty()) {
                item.intervalAlignment = MatchingService::getAlingment(item.scoreIntervalSequence, *item.midiIntervalSequence);
                item.transposition = MatchingService::getTransposition(item.scorePitchSequence, *item.midiPitchSequence, item.intervalAlignment);
                item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence, item.transposition);
            } else {
                item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence, item.transposition, saveAlignment);
            }
        }

        item.quality = MatchingService::getSongQuality(item.pitchAlignment, item.transposition);
        *i = item;
    }
    oldPitchSequence = midiPitchSequence;

    qSort(matchingItems);
    MatchingItem bestMatchingItem = matchingItems[0];
    bestMatchingItem.mergedScores = MatchingService::merge(bestMatchingItem.song.voices.value(Hand::LEFT), *bestMatchingItem.midiPairClusters, bestMatchingItem.pitchAlignment);
    emit songRecognized(bestMatchingItem);

    double bestQuality = bestMatchingItem.quality;
    double badQuality = bestQuality/5.0;
    disableBadItems(badQuality);

    bool isFinished = MatchingService::isFinished(bestMatchingItem.pitchAlignment, *bestMatchingItem.pressedSequence);
    if (isFinished) {
        midiPairClusters = QSharedPointer<QList<MidiPairCluster>>(new QList<MidiPairCluster>());
        (*midiPairClusters).append(MatchingService::cutMatchingMidiPairs(*bestMatchingItem.midiPairClusters, bestMatchingItem.pitchAlignment));
        prepareAndEmitFinishedItem(bestMatchingItem);
        init();
    }
}

void MatchingHandler::disableBadItems(const double &lowerQualityLimit) {
    for (QList<MatchingItem>::iterator i = matchingItems.begin(); i != matchingItems.end(); ++i) {
        if ((*i).quality < lowerQualityLimit) {
            (*i).enabled = false;
        }
    }
}

void MatchingHandler::prepareAndEmitFinishedItem(const MatchingItem& item) {
    MatchingItem finishedItem = item;
    *finishedItem.midiPitchSequence = MatchingService::midiPairClusters2pitchSequence(*item.midiPairClusters);
    *finishedItem.midiIntervalSequence = MatchingService::midiPairClusters2intervalSequence(*item.midiPairClusters);
    *finishedItem.pressedSequence = MatchingService::midiPairClusters2pressedSequence(*item.midiPairClusters);
    finishedItem.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence, finishedItem.transposition);
    finishedItem.intervalAlignment = MatchingService::getAlingment(item.scoreIntervalSequence, *item.midiIntervalSequence);

    finishedItem.mergedScores = MatchingService::merge(finishedItem.song.voices.value(Hand::LEFT), *finishedItem.midiPairClusters, finishedItem.pitchAlignment);

    emit songFinished(finishedItem);
}
