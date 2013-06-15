#ifndef GUIDOVIEW_H
#define GUIDOVIEW_H

#include <QList>
#include <QWidget>

#include "events.h"
#include "song.h"

class QGraphicsScene;
class QGuidoGraphicsItem;

namespace Ui {
class GuidoView;
}

class GuidoView : public QWidget
{
    Q_OBJECT
    
public:
    explicit GuidoView(QWidget *parent = 0);
    ~GuidoView();
    
protected:
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *e);

signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    
private slots:
    void playNoteOnEvent(NoteOnEvent event);
    void playNoteOffEvent(NoteOffEvent event);
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::GuidoView *ui;

    QGraphicsScene *graphicsScene;
    QGuidoGraphicsItem *guidoGraphicsItem;

    QList<Song> songs;
    Song currentSong;
};

#endif // GUIDOVIEW_H
