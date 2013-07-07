#include "feedbackmanager.h"

#include <QTimer>

FeedbackManager::FeedbackManager(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, &FeedbackManager::playNextEvent);

    state = OFF;
    echoDelay = 1000;
    hurdleVelocity = 30;
}

FeedbackManager::State FeedbackManager::getState() const {
    return state;
}

int FeedbackManager::getEchoDelay() const {
    return echoDelay;
}

int FeedbackManager::getHurdleVelocity() const {
    return hurdleVelocity;
}

void FeedbackManager::toggleOff(bool value) {
    if (value)
        state = OFF;
}

void FeedbackManager::toggleEcho(bool value) {
    if (value)
        state = ECHO;
}

void FeedbackManager::toggleReping(bool value) {
    if (value)
        state = REPING;
}

void FeedbackManager::toggleHurdle(bool value) {
    if (value)
        state = HURDLE;
}

void FeedbackManager::setEchoDelay(int value) {
    echoDelay = value;
}

void FeedbackManager::setHurdleVelocity(int value) {
    hurdleVelocity = value;
}

void FeedbackManager::playNoteOnEvent(NoteOnEvent event) {
    switch (state) {
    case OFF:
        break;
    case ECHO:
        eatChannelEvent(event);
        break;
    case REPING:
        break;
    case HURDLE:
        if (event.getVelocity() > hurdleVelocity) {
            unsigned char note = qMax(127, event.getNote()+12);
            unsigned char velocity = qMax(127, event.getVelocity()+10);
            NoteOnEvent loudOnEvent(event.getTime(), event.getChannel(), note, velocity);
            NoteOffEvent loudOffEvent(event.getTime(), event.getChannel(), note, 0);

            emit gotNoteOnEvent(loudOnEvent);
            emit gotNoteOffEvent(loudOffEvent);
        }
    }
}

void FeedbackManager::playNoteOffEvent(NoteOffEvent event) {
    switch (state) {
    case OFF:
        break;
    case ECHO:
        eatChannelEvent(event);
        break;
    case REPING:
        emit gotNoteOnEvent(NoteOnEvent(event.getTime(), event.getChannel(), event.getNote(), 128));
        events.enqueue(event);
        timer->start(PINGDELAY);
        break;
    case HURDLE:
        break;
    }
}

void FeedbackManager::eatChannelEvent(const ChannelEvent& event) {
    events.enqueue(event);
    if (!timer->isActive()) {
        timer->start(echoDelay);
    } else if (events.size() > 2) {
        int currentDelay = timer->remainingTime() + (events.last().getTime()-events.first().getTime());
        if (currentDelay > echoDelay) {
            timer->stop();
            timer->start(0);
        }
    }
}

void FeedbackManager::playNextEvent() {
    ChannelEvent event = events.dequeue();
    if (event.type() == Event::NoteOnEventType) {
        emit gotNoteOnEvent(event);
    } else if (event.type() == Event::NoteOffEventType) {
        emit gotNoteOffEvent(event);
    }

    if (events.size() > 0) {
        int delta = events.first().getTime()-event.getTime();
        timer->start(delta);
    }
}
