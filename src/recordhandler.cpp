#include "recordhandler.h"

RecordHandler::RecordHandler(QObject *parent) :
    QObject(parent), recording(false)
{
}

void RecordHandler::startRecording() {
    events.clear();
    recording = true;
}

void RecordHandler::stopRecording() {
    recording = false;
}

void RecordHandler::save() {
    if (events.size() != 0) {
        emit gotChannelEventsToSave(events);
        events.clear();
    }
}

void RecordHandler::recordChannelEvent(ChannelEvent event) {
    if (recording) {
        events.append(event);
    }
}
