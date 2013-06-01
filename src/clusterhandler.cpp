#include "clusterhandler.h"

ClusterHandler::ClusterHandler()
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &ClusterHandler::timeOut);
}

void ClusterHandler::matchNoteOnEvent(NoteOnEvent noteOn) {
    channelEvents.append(noteOn);
    if (!timer->isActive()) {
        timer->start(50);
    }
}

void ClusterHandler::matchNoteOffEvent(NoteOffEvent noteOff) {
    channelEvents.append(noteOff);
    if (!timer->isActive()) {
        timer->start(50);
    }
}

void ClusterHandler::timeOut() {
    emit gotChannelEvents(channelEvents);
    channelEvents.clear();
}
