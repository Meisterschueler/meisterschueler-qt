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

    enum State { OFF, ECHO, REPING };
    State getState() const;
    int getEchoDelay() const;
    
signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    
public slots:
    void toggleOff(bool value);
    void toggleEcho(bool value);
    void toggleReping(bool value);
    void setEchoDelay(int value);

    void playNoteOnEvent(NoteOnEvent event);
    void playNoteOffEvent(NoteOffEvent event);

private:
    QQueue<ChannelEvent> events;
    QTimer *timer;
    int echoDelay;

    State state;

    const int PINGDELAY = 100;

    void eatChannelEvent(const ChannelEvent& event);

private slots:
    void playNextEvent();
};

#endif // ECHOMANAGER_H
