#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "events.h"

#include "bubbleview.h"
#include "guidoview.h"

class MidiWrapper;
class MatchingHandler;
class MergingHandler;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void gotNoteOnEvent(NoteOnEvent);
    void gotNoteOffEvent(NoteOffEvent);

private:
    Ui::MainWindow *ui;

    MidiWrapper *midiWrapper;
    MatchingHandler *matchingHandler;
    MergingHandler *mergingHandler;
    BubbleView *bubbleView;
    GuidoView *guidoView;

    QList<Qt::Key> keys { Qt::Key_A, Qt::Key_W, Qt::Key_S, Qt::Key_E, Qt::Key_D, Qt::Key_F, Qt::Key_T, Qt::Key_G, Qt::Key_Z, Qt::Key_H, Qt::Key_U, Qt::Key_J, Qt::Key_K };

private slots:
    void on_actionBubbleView_triggered();
    void on_actionGuidoView_triggered();
    void on_actionFull_Screen_triggered();
    void on_actionSettings_triggered();
};

#endif // MAINWINDOW_H
