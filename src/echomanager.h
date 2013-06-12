#ifndef ECHOMANAGER_H
#define ECHOMANAGER_H

#include <QObject>
#include <QQueue>

#include "events.h"

class QTimer;

class EchoManager : public QObject
{
    Q_OBJECT
public:
    explicit EchoManager(QObject *parent = 0);
    
signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    
public slots:
    void enableEchoes(bool enable);
    void playNoteOnEvent(NoteOnEvent event);
    void playNoteOffEvent(NoteOffEvent event);
    
private:
    bool enabled;
    QQueue<ChannelEvent> events;
    QTimer *timer;
    int delay;

private slots:
    void playNextEvent();
};

#endif // ECHOMANAGER_H
