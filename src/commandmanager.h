#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>

#include "events.h"

class QTimer;

class CommandManager : public QObject
{
    Q_OBJECT
public:
    explicit CommandManager(QObject *parent = 0);
    
signals:
    void startRecording();
    void stopRecording();

public slots:
    void handleNoteOnEvent(NoteOnEvent event);
    void handleNoteOffEvent(NoteOffEvent event);
    void handleControlChangeEvent(ControlChangeEvent event);

private:
    ControlChangeEvent lastControlChangeEvent;
    bool recording;
    QTimer *timer;

private slots:
    void timeout();
};

#endif // COMMANDMANAGER_H
