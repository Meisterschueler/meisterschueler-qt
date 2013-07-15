#ifndef FEEDBACKMANAGER_H
#define FEEDBACKMANAGER_H

#include <QObject>
#include <QQueue>

#include "events.h"

class QTimer;

class FeedbackManager : public QObject
{
    Q_OBJECT
public:
    explicit FeedbackManager(QObject *parent = 0);

    enum State { OFF, ECHO, REPING, TUNNEL, PINGPONG };
    State getState() const;
    int getEchoDelay() const;
    int getTunnelMin() const;
    int getTunnelMax() const;
    
signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    
public slots:
    void toggleOff(bool value);
    void toggleEcho(bool value);
    void toggleReping(bool value);
    void toggleTunnel(bool value);
    void togglePingPong(bool value);

    void setEchoDelay(int value);
    void setTunnelMin(int value);
    void setTunnelMax(int value);

    void playNoteOnEvent(NoteOnEvent event);
    void playNoteOffEvent(NoteOffEvent event);

private:
    QQueue<ChannelEvent> events;
    QTimer *timer;
    int echoDelay;
    int tunnelMin;
    int tunnelMax;

    State state;

    const int PINGDELAY = 100;

    void eatChannelEvent(const ChannelEvent& event);

private slots:
    void playNextEvent();
};

#endif // FEEDBACKMANAGER_H
