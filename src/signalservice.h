#ifndef SIGNALSERVICE_H
#define SIGNALSERVICE_H

#include <QObject>

#include "events.h"

class SignalService : public QObject
{
    Q_OBJECT
public:
    explicit SignalService(QObject *parent = 0);

private:
    const QString STARTUP_SOUND = "[g1/16 {c/4,e,g,c2}]";
    const QString SHUTDOWN_SOUND = "[c2/16 b1/16 c2/4]";

    void play(const QString& gmn);

signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);

public slots:
    void playStartupSound();
    void playShutdownSound();
};

#endif // SIGNALSERVICE_H
