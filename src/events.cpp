#include "events.h"

const QEvent::Type Event::NoteOffEventType = static_cast<QEvent::Type>(QEvent::registerEventType( QEvent::User + STATUS_NOTEOFF ));
const QEvent::Type Event::NoteOnEventType = static_cast<QEvent::Type>(QEvent::registerEventType( QEvent::User + STATUS_NOTEON ));
const QEvent::Type Event::PolyKeyPressEventType = static_cast<QEvent::Type>(QEvent::registerEventType( QEvent::User + STATUS_POLYAFT ));
const QEvent::Type Event::ControlChangeEventType = static_cast<QEvent::Type>(QEvent::registerEventType( QEvent::User + STATUS_CTLCHG ));
const QEvent::Type Event::ProgramChangeEventType = static_cast<QEvent::Type>(QEvent::registerEventType( QEvent::User + STATUS_PROGRAM ));
const QEvent::Type Event::ChannelKeyPressEventType = static_cast<QEvent::Type>(QEvent::registerEventType( QEvent::User + STATUS_CHANAFT ));
const QEvent::Type Event::PitchWheelEventType = static_cast<QEvent::Type>(QEvent::registerEventType( QEvent::User + STATUS_BENDER));


