/*
    MIDI Virtual Piano Keyboard
    Copyright (C) 2008-2013, Pedro Lopez-Cabanillas <plcl@users.sf.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EVENTS_H
#define EVENTS_H

#include "mididefs.h"
#include <QEvent>
#include <QSharedPointer>

class Event : public QEvent
{
public:
    Event(const QEvent::Type& type) : QEvent(type) {}

    static const QEvent::Type NoteOffEventType;
    static const QEvent::Type NoteOnEventType;
    static const QEvent::Type PolyKeyPressEventType;
    static const QEvent::Type ControlChangeEventType;
    static const QEvent::Type ProgramChangeEventType;
    static const QEvent::Type ChannelKeyPressEventType;
    static const QEvent::Type PitchWheelEventType;
};

class NoteEvent : public Event
{
public:
    NoteEvent(time_t time, unsigned char note, unsigned char vel, Event::Type type)
        : Event(type), m_time(time), m_note(note), m_velocity(vel) { }
    time_t getTime() const { return m_time; }
    unsigned char getNote() const { return m_note; }
    unsigned char getVelocity() const { return m_velocity; }

protected:
    time_t m_time;
    unsigned char m_note;
    unsigned char m_velocity;
};

class ChannelEvent : public NoteEvent
{
public:
    ChannelEvent() : NoteEvent(0, 0, 0, Event::User) {}
    ChannelEvent(time_t time, unsigned char chan, unsigned char note, unsigned char vel, Event::Type type)
        : NoteEvent(time, note, vel, type), m_channel(chan) { }
    unsigned char getChannel() const { return m_channel; }

    bool operator==(ChannelEvent const& rhs) const {
        return (this->m_channel == rhs.m_channel && this->m_note == rhs.m_note && this->m_time == rhs.m_time && this->m_velocity == rhs.m_velocity);
    }

    bool operator!=(ChannelEvent const& rhs) const {
        return (this->m_channel != rhs.m_channel || this->m_note != rhs.m_note || this->m_time != rhs.m_time || this->m_velocity != rhs.m_velocity);
    }

    bool operator<(ChannelEvent const& rhs) const {
        return this->m_time<rhs.getTime();
    }

protected:
    unsigned char m_channel;
};

Q_DECLARE_METATYPE(ChannelEvent)


class NoteOnEvent : public ChannelEvent
{
public:
    NoteOnEvent() : ChannelEvent(0, 0, 0, 0, NoteOnEventType) {}
    NoteOnEvent(ChannelEvent e) : ChannelEvent(e.getTime(), e.getChannel(), e.getNote(), e.getVelocity(), NoteOnEventType) {}
    NoteOnEvent(time_t time, unsigned char chan, unsigned char note, unsigned char vel)
        : ChannelEvent(time, chan, note, vel, NoteOnEventType) { }

    bool operator<(const NoteOnEvent& rhs) const {
        return this->m_note<rhs.getNote();
    }
};

Q_DECLARE_METATYPE(NoteOnEvent)


class NoteOffEvent : public ChannelEvent
{
public:
    NoteOffEvent() : ChannelEvent(0, 0, 0, 0, NoteOffEventType) {}
    NoteOffEvent(ChannelEvent e) : ChannelEvent(e.getTime(), e.getChannel(), e.getNote(), e.getVelocity(), NoteOffEventType) {}
    NoteOffEvent(time_t time, unsigned char chan, unsigned char note, unsigned char vel)
        : ChannelEvent(time, chan, note, vel, NoteOffEventType) { }

    bool operator<(NoteOffEvent const& rhs) const {
        return this->m_note<rhs.getNote();
    }
};

Q_DECLARE_METATYPE(NoteOffEvent)


class PolyKeyPressEvent : public NoteEvent
{
public:
    PolyKeyPressEvent(time_t time, unsigned char note, unsigned char value)
        : NoteEvent(time, note, value, PolyKeyPressEventType) { }
};

class ControlChangeEvent : public Event
{
public:
    ControlChangeEvent() : Event(ControlChangeEventType), m_time(0), m_ctl(0), m_value(0) {}
    ControlChangeEvent(time_t time, unsigned char ctl, unsigned char value)
        : Event(ControlChangeEventType), m_time(time), m_ctl(ctl), m_value(value) { }
    time_t getTime() const { return m_time; }
    unsigned char getController() const { return m_ctl; }
    unsigned char getValue() const { return m_value; }
private:
    time_t m_time;
    unsigned char m_ctl;
    unsigned char m_value;
};

Q_DECLARE_METATYPE(ControlChangeEvent)


class ValueEvent : public Event
{
public:
    ValueEvent(int value, Event::Type type)
        : Event(type), m_value(value) { }
    int getValue() const { return m_value; }
private:
    int m_value;
};

class ProgramChangeEvent : public ValueEvent
{
public:
    ProgramChangeEvent(unsigned char value)
        : ValueEvent(value, ProgramChangeEventType) { }
};

class ChannelKeyPressEvent : public ValueEvent
{
public:
    ChannelKeyPressEvent(unsigned char value)
        : ValueEvent(value, ChannelKeyPressEventType) { }
};

class PitchWheelEvent : public ValueEvent
{
public:
    PitchWheelEvent(int value)
        : ValueEvent(value, PitchWheelEventType) { }
};

static NoteOnEvent emptyNoteOnEvent(0, 255, 0, 0);
static NoteOnEvent emptyNoteOffEvent(0, 255, 0, 0);

class MidiPair {
public:
    NoteOnEvent noteOn;
    NoteOffEvent noteOff;

    MidiPair() : noteOn(emptyNoteOnEvent), noteOff(emptyNoteOffEvent) {}
    MidiPair(NoteOnEvent noteOn) : noteOn(noteOn), noteOff(emptyNoteOffEvent) {}
    MidiPair(NoteOnEvent noteOn, NoteOffEvent noteOff) : noteOn(noteOn), noteOff(noteOff) {}

    bool operator==(const MidiPair& rhs) const {
        return (noteOn == rhs.noteOn && noteOff == rhs.noteOff);
    }

    bool operator!=(const MidiPair& rhs) const {
        return (noteOn != rhs.noteOn || noteOff != rhs.noteOff);
    }

    bool operator<(const MidiPair& rhs) const {
        return (this->noteOn.getNote() < rhs.noteOn.getNote() );
    }
};

class MidiPairCluster {
public:
    time_t time;
    QList<MidiPair> midiPairs;

    bool operator<(const MidiPairCluster& rhs) const {
        return (this->time < rhs.time);
    }

    bool operator!=(const MidiPairCluster& rhs) const {
        if (this->time!=rhs.time || this->midiPairs.count() != rhs.midiPairs.count()) {
            return true;
        } else {
            for (int var = 0; var < midiPairs.count(); ++var) {
                if (midiPairs.at(var) != rhs.midiPairs.at(var)) {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif /* EVENTS_H */
