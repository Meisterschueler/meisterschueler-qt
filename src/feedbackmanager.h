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

    enum State { OFF, ECHO, REPING, HURDLE };
    State getState() const;
    int getEchoDelay() const;
    int getHurdleVelocity() const;
    
signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    
public slots:
    void toggleOff(bool value);
    void toggleEcho(bool value);
    void toggleReping(bool value);
    void toggleHurdle(bool value);

    void setEchoDelay(int value);
    void setHurdleVelocity(int value);

    void playNoteOnEvent(NoteOnEvent event);
    void playNoteOffEvent(NoteOffEvent event);

private:
    QQueue<ChannelEvent> events;
    QTimer *timer;
    int echoDelay;
    int hurdleVelocity;

    State state;

    const int PINGDELAY = 100;

    void eatChannelEvent(const ChannelEvent& event);

private slots:
    void playNextEvent();
};

#endif // FEEDBACKMANAGER_H
