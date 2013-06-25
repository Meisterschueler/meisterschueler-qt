#include "commandmanager.h"

#include <QTimer>

CommandManager::CommandManager(QObject *parent) :
    QObject(parent), recording(false)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(50);

    QObject::connect(timer, &QTimer::timeout, this, &CommandManager::timeout);
}

void CommandManager::handleNoteOnEvent(NoteOnEvent event) {

}

void CommandManager::handleNoteOffEvent(NoteOffEvent event) {

}

void CommandManager::handleControlChangeEvent(ControlChangeEvent event) {
    lastControlChangeEvent = event;
    if (!timer->isActive()) {
        timer->start();
    }
}

void CommandManager::timeout() {
    switch (lastControlChangeEvent.getController()) {
    case 64: // right pedal
        break;
    case 67: // left pedal
        if (lastControlChangeEvent.getValue() == 127) {
            if (recording) {
                recording = false;
                emit stopRecording();
            } else {
                recording = true;
                emit startRecording();
            }
        }
        break;
    default:
        break;
    }
}
