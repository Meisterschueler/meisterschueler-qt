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

const QEvent::Type NoteOffEventType = QEvent::Type(
        QEvent::registerEventType( QEvent::User + STATUS_NOTEOFF ) );
const QEvent::Type NoteOnEventType = QEvent::Type(
        QEvent::registerEventType( QEvent::User + STATUS_NOTEON ) );
const QEvent::Type PolyKeyPressEventType = QEvent::Type(
        QEvent::registerEventType( QEvent::User + STATUS_POLYAFT ) );
const QEvent::Type ControlChangeEventType = QEvent::Type(
        QEvent::registerEventType( QEvent::User + STATUS_CTLCHG ) );
const QEvent::Type ProgramChangeEventType = QEvent::Type(
        QEvent::registerEventType( QEvent::User + STATUS_PROGRAM ) );
const QEvent::Type ChannelKeyPressEventType = QEvent::Type(
        QEvent::registerEventType( QEvent::User + STATUS_CHANAFT ) );
const QEvent::Type PitchWheelEventType = QEvent::Type(
        QEvent::registerEventType( QEvent::User + STATUS_BENDER) );

class NoteEvent : public QEvent
{
public:
    NoteEvent(time_t time, unsigned char note, unsigned char vel, QEvent::Type type)
        : QEvent(type), m_time(time), m_note(note), m_velocity(vel) { }
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
    ChannelEvent(time_t time, unsigned char chan, unsigned char note, unsigned char vel, QEvent::Type type)
        : NoteEvent(time, note, vel, type), m_channel(chan) { }
    unsigned char getChannel() const { return m_channel; }

    bool operator==(ChannelEvent const& rhs) {
        if (this->m_channel == rhs.m_channel && this->m_note == rhs.m_note && this->m_time == rhs.m_time && this->m_velocity == rhs.m_velocity) {
            return true;
        } else {
            return false;
        }
    }

protected:
    unsigned char m_channel;
};

class NoteOffEvent : public ChannelEvent
{
public:
    NoteOffEvent(time_t time, unsigned char chan, unsigned char note, unsigned char vel)
        : ChannelEvent(time, chan, note, vel, NoteOffEventType) { }
};

class NoteOnEvent : public ChannelEvent
{
public:
    NoteOnEvent(time_t time, unsigned char chan, unsigned char note, unsigned char vel)
        : ChannelEvent(time, chan, note, vel, NoteOnEventType) { }

    int getDegree() const { return m_note % 12; }
    int getType() const
    {
        int g = getDegree();
        if (g == 1 || g == 3 || g == 6 || g == 8 || g == 10)
            return 1;
        return 0;
    }
};

class PolyKeyPressEvent : public NoteEvent
{
public:
    PolyKeyPressEvent(time_t time, unsigned char note, unsigned char value)
        : NoteEvent(time, note, value, PolyKeyPressEventType) { }
};

class ControlChangeEvent : public QEvent
{
public:
    ControlChangeEvent(time_t time, unsigned char ctl, unsigned char value)
        : QEvent(ControlChangeEventType), m_time(time), m_ctl(ctl), m_value(value) { }
    time_t getTime() const { return m_time; }
    unsigned char getController() const { return m_ctl; }
    unsigned char getValue() const { return m_value; }
private:
    time_t m_time;
    unsigned char m_ctl;
    unsigned char m_value;
};

class ValueEvent : public QEvent
{
public:
    ValueEvent(int value, QEvent::Type type)
        : QEvent(type), m_value(value) { }
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

class NoteEventPair {
public:
    NoteEventPair() {}

    QSharedPointer<NoteOnEvent> noteOn;
    QSharedPointer<NoteOffEvent> noteOff;

    bool operator<(const NoteEventPair& rhs) const {
        if (this->noteOn && rhs.noteOn && this->noteOn->getTime() < rhs.noteOn->getTime()) {
            return true;
        } else {
            return false;
        }
    }
};

#endif /* EVENTS_H */
