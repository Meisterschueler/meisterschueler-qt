#ifndef RECORDHANDLER_H
#define RECORDHANDLER_H

#include <QList>
#include <QObject>

#include "events.h"

class RecordHandler : public QObject
{
    Q_OBJECT
public:
    explicit RecordHandler(QObject *parent = 0);
    
signals:
    void gotChannelEventsToSave(const QList<ChannelEvent>& events);

public slots:
    void startRecording();
    void stopRecording();
    void save();

    void recordChannelEvent(ChannelEvent event);

private:
    bool recording;
    QList<ChannelEvent> events;
};

#endif // RECORDHANDLER_H
