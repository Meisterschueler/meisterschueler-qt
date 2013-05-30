#include "matchinghandler.h"

#include "matchingservice.h"
#include "midiservice.h"
#include "needlemanwunsch.h"

MatchingHandler::MatchingHandler(const QList<MatchingItem> &matchingItems) : matchingItems(matchingItems)
{
    midiPairs = QSharedPointer<QList<MidiPair>>(new QList<MidiPair>());
}

void MatchingHandler::reset() {
    for (QList<MatchingItem>::iterator i = matchingItems.begin(); i != matchingItems.end(); ++i) {
        (*i).enabled = true;
    }
}

void MatchingHandler::matchNoteOnEvent(NoteOnEvent noteOn) {
    MidiService::addNoteOn(*midiPairs, noteOn);
    match();
}

void MatchingHandler::matchNoteOffEvent(NoteOffEvent noteOff) {
    MidiService::addNoteOff(*midiPairs, noteOff);
    match();
}

void MatchingHandler::matchChannelEvents(QList<ChannelEvent> channelEvents) {
    for (ChannelEvent channelEvent : channelEvents) {
        if (channelEvent.type() == Event::NoteOnEventType) {
            MidiService::addNoteOn(*midiPairs, channelEvent);
        } else if (channelEvent.type() == Event::NoteOffEventType) {
            MidiService::addNoteOff(*midiPairs, channelEvent);
        }
    }
    match();
}

void MatchingHandler::match() {
    static QSharedPointer<QByteArray> oldPitchSequence;

    QSharedPointer<QByteArray> midiPitchSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiPairs2pitchSequence(*midiPairs)));
    QSharedPointer<QByteArray> midiIntervalSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiPairs2intervalSequence(*midiPairs)));
    QSharedPointer<QByteArray> pressedSequence = QSharedPointer<QByteArray>(new QByteArray(MatchingService::midiPairs2pressedSequence(*midiPairs)));

    bool pitchSequenceChanged = !oldPitchSequence || !(*oldPitchSequence == *midiPitchSequence);
    oldPitchSequence = midiPitchSequence;

    for (QList<MatchingItem>::iterator i = matchingItems.begin(); i != matchingItems.end(); ++i) {
        MatchingItem item = *i;
        if (!item.enabled) {
            continue;
        }

        item.midiPairs = midiPairs;
        item.midiPitchSequence = midiPitchSequence;
        item.midiIntervalSequence = midiIntervalSequence;
        item.pressedSequence = pressedSequence;

        if (pitchSequenceChanged) {
            QByteArray saveAlignment = MatchingService::getSaveAlignment(item.pitchAlignment);

            if (saveAlignment.isEmpty()) {
                item.intervalAlignment = MatchingService::getAlingment(item.scoreIntervalSequence, *item.midiIntervalSequence);
                item.transposition = MatchingService::getTransposition(item.scorePitchSequence, *item.midiPitchSequence, item.intervalAlignment);
                item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence, item.transposition);
            } else {
                item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence, item.transposition, saveAlignment);
            }
        }

        item.quality = MatchingService::getQuality(item.pitchAlignment, item.transposition);
        *i = item;
    }

    qSort(matchingItems);
    MatchingItem item = matchingItems[0];

    double bestQuality = item.quality;
    double badQuality = bestQuality/5.0;
    for (QList<MatchingItem>::iterator i = matchingItems.begin(); i != matchingItems.end(); ++i) {
        if ((*i).quality < badQuality) {
            (*i).enabled = false;
        }
    }

    bool isFinished = MatchingService::isFinished(item.pitchAlignment, *item.pressedSequence);
    if (isFinished) {

        midiPairs = QSharedPointer<QList<MidiPair>>(new QList<MidiPair>());

        (*midiPairs).append(MatchingService::cutMatchingMidiPairs(*item.midiPairs, item.pitchAlignment));

        *item.midiPitchSequence = MatchingService::midiPairs2pitchSequence(*item.midiPairs);
        *item.midiIntervalSequence = MatchingService::midiPairs2intervalSequence(*item.midiPairs);
        *item.pressedSequence = MatchingService::midiPairs2pressedSequence(*item.midiPairs);
        item.pitchAlignment = MatchingService::getAlingment(item.scorePitchSequence, *item.midiPitchSequence, item.transposition);
        item.intervalAlignment = MatchingService::getAlingment(item.scoreIntervalSequence, *item.midiIntervalSequence);

        emit songFinished(item);
        reset();
    }
}
