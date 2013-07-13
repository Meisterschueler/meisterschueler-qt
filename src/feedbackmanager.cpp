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
    tunnelMin = 0;
    tunnelMax = 127;
}

FeedbackManager::State FeedbackManager::getState() const {
    return state;
}

int FeedbackManager::getEchoDelay() const {
    return echoDelay;
}

int FeedbackManager::getTunnelMin() const {
    return tunnelMin;
}

int FeedbackManager::getTunnelMax() const {
    return tunnelMax;
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

void FeedbackManager::toggleTunnel(bool value) {
    if (value)
        state = TUNNEL;
}

void FeedbackManager::setEchoDelay(int value) {
    echoDelay = value;
}

void FeedbackManager::setTunnelMin(int value) {
    tunnelMin = value;
}

void FeedbackManager::setTunnelMax(int value) {
    tunnelMax = value;
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
    case TUNNEL:
        if (event.getVelocity() > tunnelMin && event.getVelocity() < tunnelMax) {
            unsigned char note = 127;
            unsigned char velocity = 127;
            NoteOnEvent loudOnEvent(0, 0, note, velocity);
            NoteOffEvent loudOffEvent(0, 0, note, 0);

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
    case TUNNEL:
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
