#ifndef CLUSTERHANDLER_H
#define CLUSTERHANDLER_H

#include <QList>
#include <QObject>
#include <QTimer>

#include "events.h"

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

private:
    QTimer *timer;
    QList<ChannelEvent> channelEvents;

private slots:
    void timeOut();
};

#endif // CLUSTERHANDLER_H
