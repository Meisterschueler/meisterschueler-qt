#ifndef MIDIWRAPPER_H
#define MIDIWRAPPER_H

#include <QObject>

#include "events.h"

class RtMidiIn;
class RtMidiOut;

class MidiWrapper : public QObject
{
    Q_OBJECT
public:
    explicit MidiWrapper(QObject *parent = 0);
    ~MidiWrapper();

    QStringList getInputPorts() const;
    QStringList getOutputPorts() const;

    QString getOpenedInputPort() const;
    QString getOpenedOutputPort() const;

protected:
    RtMidiIn *midiIn;
    RtMidiOut *midiOut;

private:
    QString openedInputPort;
    QString openedOutputPort;
    
signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    void gotControlChangeEvent(ControlChangeEvent event);

protected:
    void customEvent(QEvent *event);

public slots:
    void playNoteOn(NoteOnEvent event);
    void playNoteOff(NoteOffEvent event);

    void openInputPort(QString portName);
    void openOutputPort(QString portName);
    
};

#endif // MIDIWRAPPER_H
