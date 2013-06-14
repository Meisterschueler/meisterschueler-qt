#include "echomanager.h"

#include <QTimer>

EchoManager::EchoManager(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, &EchoManager::playNextEvent);

    state = OFF;
    echoDelay = 1000;
}

EchoManager::State EchoManager::getState() const {
    return state;
}

int EchoManager::getEchoDelay() const {
    return echoDelay;
}

void EchoManager::toggleOff(bool value) {
    if (value)
        state = OFF;
}

void EchoManager::toggleEcho(bool value) {
    if (value)
        state = ECHO;
}

void EchoManager::toggleReping(bool value) {
    if (value)
        state = REPING;
}

void EchoManager::setEchoDelay(int value) {
    echoDelay = value;
}

void EchoManager::playNoteOnEvent(NoteOnEvent event) {
    switch (state) {
    case OFF:
        break;
    case ECHO:
        eatChannelEvent(event);
        break;
    case REPING:
        break;
    }
}

void EchoManager::playNoteOffEvent(NoteOffEvent event) {
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
    }
}

void EchoManager::eatChannelEvent(const ChannelEvent& event) {
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

void EchoManager::playNextEvent() {
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
