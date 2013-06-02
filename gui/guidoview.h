#ifndef GUIDOVIEW_H
#define GUIDOVIEW_H

#include <QList>
#include <QWidget>

class QGraphicsScene;
class QGuidoGraphicsItem;

class Song;

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
    
private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::GuidoView *ui;

    QGraphicsScene *graphicsScene;
    QGuidoGraphicsItem *guidoGraphicsItem;

    QList<Song> songs;
};

#endif // GUIDOVIEW_H
