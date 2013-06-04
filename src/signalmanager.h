#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include <QObject>

#include "events.h"

class QTimer;

class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = 0);

private:
    const QString STARTUP_SOUND = "[g1/16 {c/4,e,g,c2}]";
    const QString SHUTDOWN_SOUND = "[c2/16 b1/16 c2/4]";
    const QString FINISHED_SOUND = "[c2]";

    int idx;
    QList<ChannelEvent> events;
    QTimer *timer;

    void play(const QString& gmn);

signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);

public slots:
    void playStartupSound();
    void playShutdownSound();

    void playFinishedSound();

private slots:
    void playNextEvent();
};

#endif // SIGNALMANAGER_H
