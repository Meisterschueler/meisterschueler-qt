#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "bubbleview.h"
#include "guidoview.h"
#include "midiwrapper.h"

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
    
private:
    Ui::MainWindow *ui;

    MidiWrapper *midiWrapper;
    BubbleView *bubbleView;
    GuidoView *guidoView;

public slots:
    void toggleFullscreen();

    void showSettingsDialog();

private slots:
    void on_actionBubbleView_triggered();
    void on_actionGuidoView_triggered();
};

#endif // MAINWINDOW_H
