#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "events.h"

class BubbleView;
class GuidoView;

class ClusterHandler;
class CommandManager;
class FeedbackManager;
class MidiWrapper;
class MatchingHandler;
class MergingHandler;
class PlaybackHandler;
class RecordHandler;
class ResultManager;
class SignalManager;

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

    void previousPage();
    void nextPage();

private:
    Ui::MainWindow *ui;

    ClusterHandler *midiClusterHandler;
    ClusterHandler *dummyClusterHandler;
    CommandManager *commandManager;
    FeedbackManager *feedbackManager;
    MidiWrapper *midiWrapper;
    MatchingHandler *matchingHandler;
    MergingHandler *mergingHandler;
    PlaybackHandler *playbackHandler;
    RecordHandler *recordHandler;
    ResultManager *resultManager;
    SignalManager *signalManager;

    BubbleView *bubbleView;
    GuidoView *guidoView;

    QList<Qt::Key> keys { Qt::Key_A, Qt::Key_W, Qt::Key_S, Qt::Key_E, Qt::Key_D, Qt::Key_F, Qt::Key_T, Qt::Key_G, Qt::Key_Z, Qt::Key_H, Qt::Key_U, Qt::Key_J, Qt::Key_K };

private slots:
    void on_actionBubbleView_triggered();
    void on_actionGuidoView_triggered();
    void on_actionFull_Screen_triggered();
    void on_actionSettings_triggered();
    void on_actionLoad_File_triggered();
    void on_actionChordWidget_triggered();
    void on_actionExit_triggered();
    void on_actionFeedback_triggered();
    void on_actionActionQCustomPlot_triggered();
    void on_actionTimelineView_triggered();
};

#endif // MAINWINDOW_H
