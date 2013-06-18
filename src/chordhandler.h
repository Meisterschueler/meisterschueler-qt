#ifndef CHORDHANDLER_H
#define CHORDHANDLER_H

#include <QMap>
#include <QObject>

class ChordHandler : public QObject
{
    Q_OBJECT
public:
    explicit ChordHandler(QObject *parent = 0);

private:
    QMap<QString, QString> chords;
    QMap<QString, QString> scales;
    
signals:
    
public slots:

};

#endif // CHORDHANDLER_H
