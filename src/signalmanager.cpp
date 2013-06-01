#include "signalmanager.h"

SignalManager::SignalManager(QObject *parent) :
    QObject(parent)
{
}

void SignalManager::play(const QString& gmn) {

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
