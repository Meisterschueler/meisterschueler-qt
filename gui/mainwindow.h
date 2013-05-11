#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../src/midiwrapper.h"

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

public slots:
    void toggleFullscreen();

    void showSettingsDialog();
};

#endif // MAINWINDOW_H
