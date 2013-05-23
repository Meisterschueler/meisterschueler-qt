#include "matchinghandler.h"

#include "matchingservice.h"
#include "midiservice.h"
#include "needlemanwunsch.h"

MatchingHandler::MatchingHandler(const QList<MatchingItem> &matchingItems) : matchingItems(matchingItems)
{
    midiEvents = QSharedPointer<QList<NoteEventPair>>(new QList<NoteEventPair>());
}

void MatchingHandler::reset() {

}

void MatchingHandler::noteOnEvent(NoteOnEvent noteOn) {
    MidiService::addNoteOn(*midiEvents, noteOn);
    match();
}

void MatchingHandler::noteOffEvent(NoteOffEvent noteOff) {
    MidiService::addNoteOff(*midiEvents, noteOff);
    match();
}

void MatchingHandler::match() {
    static QSharedPointer<QByteArray> oldPitchSequence;

    QSharedPointer<QByteArray> midiPitchSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiEvents2pitchSequence(*midiEvents)));
    QSharedPointer<QByteArray> midiIntervalSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiEvents2intervalSequence(*midiEvents)));
    QSharedPointer<QByteArray> pressedSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiEvents2pressedSequence(*midiEvents)));

    bool pitchSequenceChanged = !oldPitchSequence || !(*oldPitchSequence == *midiPitchSequence);
    oldPitchSequence = midiPitchSequence;

    //for (MatchingItem item : matchingItems) {
    for (QList<MatchingItem>::iterator i = matchingItems.begin(); i != matchingItems.end(); ++i) {
        MatchingItem item = *i;
        item.midiEvents = midiEvents;
        item.midiPitchSequence = midiPitchSequence;
        item.midiIntervalSequence = midiIntervalSequence;
        item.pressedSequence = pressedSequence;

        if (pitchSequenceChanged) {
            QByteArray saveAlignment = MatchingService::getSaveAlignment(item.pitchAlignment);

            if (saveAlignment.isEmpty()) {
                item.intervalAlignment = MatchingService::getAlingment(item.scoreIntervalSequence, *item.midiIntervalSequence);
                item.transposition = MatchingService::getTransposition(item.scorePitchSequence, *item.midiPitchSequence, item.intervalAlignment);
                item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence);
            } else {
                item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence, item.pitchAlignment);
            }
        }

        item.quality = MatchingService::getQuality(item.pitchAlignment, item.transposition);
        *i = item;
    }

    qSort(matchingItems);
    MatchingItem item = matchingItems[0];

    bool isFinished = MatchingService::isFinished(item.pitchAlignment, *item.pressedSequence);
    if (isFinished) {

        midiEvents = QSharedPointer<QList<NoteEventPair>>(new QList<NoteEventPair>());

        (*midiEvents).append(MatchingService::cutMatchingMidiEvents(*item.midiEvents, item.pitchAlignment));

        *item.midiPitchSequence = MatchingService::midiEvents2pitchSequence(*item.midiEvents);
        *item.midiIntervalSequence = MatchingService::midiEvents2intervalSequence(*item.midiEvents);
        *item.pressedSequence = MatchingService::midiEvents2pressedSequence(*item.midiEvents);
        item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence);
        item.intervalAlignment = MatchingService::getAlingment(item.scoreIntervalSequence, *item.midiIntervalSequence);

        emit songFinished(item);
    }
}
