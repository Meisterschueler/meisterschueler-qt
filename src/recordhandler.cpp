#include "recordhandler.h"

#include "midiservice.h"

#include <QDate>
#include <QTime>

RecordHandler::RecordHandler(QObject *parent) :
    QObject(parent), recording(false)
{
}

void RecordHandler::startRecording() {
    channelEvents.clear();
    recording = true;
}

void RecordHandler::stopRecording() {
    recording = false;
}

void RecordHandler::save() {
    if (channelEvents.size() != 0) {
        QString filename = "Record_" + QDate::currentDate().toString("yyMMdd") + QTime::currentTime().toString("HHmmss");
        MidiService::save(filename, channelEvents);

        channelEvents.clear();
    }
}

void RecordHandler::recordChannelEvent(ChannelEvent event) {
    if (recording) {
        channelEvents.append(event);
    }
}
