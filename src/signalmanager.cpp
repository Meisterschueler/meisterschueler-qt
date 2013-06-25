#include "signalmanager.h"

#include <QList>
#include <QTimer>

#include "guidoservice.h"
#include "score.h"

SignalManager::SignalManager(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, &SignalManager::playNextEvent);
}

void SignalManager::play(const QString& gmn) {
    QList<Score> scores = GuidoService::gmnToScores(gmn);
    events.clear();

    double bpm = 120.0;
    double fraction2ms = (double)Fraction(1,4) * 60.0 / bpm * 1000.0;

    for (Score score : scores) {
        double position = fraction2ms * (double)score.position;
        double duration = fraction2ms * (double)score.duration;

        events.append(NoteOnEvent(position, 0, score.pitch, 50));
        events.append(NoteOffEvent(position+duration, 0, score.pitch, 0));
    }

    qSort(events);

    idx=0;
    timer->start(0);
}

void SignalManager::playStartupSound() {
    play(STARTUP_SOUND);
}

void SignalManager::playShutdownSound() {
    play(SHUTDOWN_SOUND);
}

void SignalManager::playFinishedSound() {
    play(FINISHED_SOUND);
}

void SignalManager::playResetSound() {
    play(RESET_SOUND);
}

void SignalManager::playRecordStartSignal() {
    play(RECORD_START_SOUND);
}

void SignalManager::playRecordStopSignal() {
    play(RECORD_STOP_SOUND);
}

void SignalManager::playNextEvent() {
    if (events.at(idx).type() == Event::NoteOnEventType) {
        emit gotNoteOnEvent(NoteOnEvent(events.at(idx)));
    } else if (events.at(idx).type() == Event::NoteOffEventType) {
        emit gotNoteOffEvent(NoteOffEvent(events.at(idx)));
    }

    if (idx+1 < events.size()) {
        int delta = events.at(idx+1).getTime() - events.at(idx).getTime();
        idx++;
        timer->start(delta);
    }
}
