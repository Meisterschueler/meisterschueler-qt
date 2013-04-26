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

    QStringList getInputPorts();
    QStringList getOutputPorts();

    QString getOpenedInputPort();
    QString getOpenedOutputPort();

protected:
    RtMidiIn *midiIn;
    RtMidiOut *midiOut;

private:
    QString openedInputPort;
    QString openedOutputPort;
    
signals:
    void noteOnEvent(NoteOnEvent event);
    void noteOffEvent(NoteOffEvent event);
    void controlChangeEvent(ControlChangeEvent event);

protected:
    void customEvent(QEvent *event);

public slots:
    void noteOn(NoteOnEvent event);
    void noteOff(NoteOffEvent event);

    void openInputPort(QString portName);
    void openOutputPort(QString portName);
    
};

#endif // MIDIWRAPPER_H
