#include "signalmanager.h"

#include <QList>

#include "guidoservice.h"
#include "score.h"

SignalManager::SignalManager(QObject *parent) :
    QObject(parent)
{
}

void SignalManager::play(const QString& gmn) {
    QList<Score> scores = GuidoService::gmnToScores(gmn);
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
