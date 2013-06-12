#include "echomanager.h"

#include <QTimer>

EchoManager::EchoManager(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, &EchoManager::playNextEvent);

    delay = 1000;
}

void EchoManager::enableEchoes(bool enable) {
    enabled = enable;
}

void EchoManager::playNoteOn(NoteOnEvent event) {
    events.enqueue(event);
    if (enabled && !timer->isActive()) {
        timer->start(delay);
    }
}

void EchoManager::playNoteOff(NoteOffEvent event) {
    events.enqueue(event);
    if (enabled && !timer->isActive()) {
        timer->start(delay);
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
