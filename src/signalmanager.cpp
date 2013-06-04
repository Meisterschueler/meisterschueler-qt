#include "signalmanager.h"

#include <QList>
#include <QTimer>

#include "guidoservice.h"
#include "score.h"

SignalManager::SignalManager(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &SignalManager::playNextEvent);
}

void SignalManager::play(const QString& gmn) {
    QList<Score> scores = GuidoService::gmnToScores(gmn);
    events.clear();
    for (Score score : scores) {
        double position = (double)score.position.getNumerator()/score.position.getDenominator();
        double duration = (double)score.duration.getNumerator()/score.duration.getDenominator();

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
