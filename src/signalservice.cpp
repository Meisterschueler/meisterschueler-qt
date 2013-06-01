#include "signalservice.h"

SignalService::SignalService(QObject *parent) :
    QObject(parent)
{
}

void SignalService::play(const QString& gmn) {

}

void SignalService::playStartupSound() {
    play(STARTUP_SOUND);
}

void SignalService::playShutdownSound() {
    play(SHUTDOWN_SOUND);
}
