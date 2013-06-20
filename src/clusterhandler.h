#ifndef CLUSTERHANDLER_H
#define CLUSTERHANDLER_H

#include <QList>
#include <QObject>

#include "events.h"

class QTimer;

class ClusterHandler : public QObject
{
    Q_OBJECT
public:
    ClusterHandler();

public slots:
    void matchNoteOnEvent(NoteOnEvent noteOn);
    void matchNoteOffEvent(NoteOffEvent noteOff);

signals:
    void gotChannelEvents(QList<ChannelEvent>  events);
    void reset();

private:
    QTimer *delayTimer;
    QTimer *resetTimer;
    QList<ChannelEvent> channelEvents;
    bool pressedKeys[128];

private slots:
    void timeOutDelay();
    void timeOutReset();
};

#endif // CLUSTERHANDLER_H
