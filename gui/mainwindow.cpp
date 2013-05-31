#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    midiWrapper = new MidiWrapper();
    bubbleView = new BubbleView();
    guidoView = new GuidoView();

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    QObject::connect(ui->actionFull_Screen, SIGNAL(triggered()), this, SLOT(toggleFullscreen()));
    QObject::connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    QObject::connect(this, SIGNAL(gotNoteOnEvent(NoteOnEvent)), bubbleView, SLOT(showNoteOnEvent(NoteOnEvent)));

    QObject::connect(this, SIGNAL(gotNoteOnEvent(NoteOnEvent)), midiWrapper, SLOT(playNoteOn(NoteOnEvent)));
    QObject::connect(this, SIGNAL(gotNoteOffEvent(NoteOffEvent)), midiWrapper, SLOT(playNoteOff(NoteOffEvent)));

    QObject::connect(midiWrapper, SIGNAL(gotNoteOnEvent(NoteOnEvent)), bubbleView, SLOT(showNoteOnEvent(NoteOnEvent)));

    QObject::connect(bubbleView, SIGNAL(gotNoteOnEvent(NoteOnEvent)), midiWrapper, SLOT(playNoteOn(NoteOnEvent)));
    QObject::connect(bubbleView, SIGNAL(gotNoteOffEvent(NoteOffEvent)), midiWrapper, SLOT(playNoteOff(NoteOffEvent)));

    setCentralWidget(bubbleView);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete midiWrapper;
    delete bubbleView;
    delete guidoView;
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int idx = keys.indexOf((Qt::Key)event->key());
    int offset = -1;
    if (event->modifiers() == Qt::NoModifier) {
        offset = 48;
    } else if (event->modifiers() == Qt::ShiftModifier) {
        offset = 60;
    }

    if ( idx != -1 && !event->isAutoRepeat() ) {
        emit gotNoteOnEvent(NoteOnEvent(0, 0, offset+idx, 50));
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    int idx = keys.indexOf((Qt::Key)event->key());
    int offset = -1;
    if (event->modifiers() == Qt::NoModifier) {
        offset = 48;
    } else if (event->modifiers() == Qt::ShiftModifier) {
        offset = 60;
    }

    if ( idx != -1 && !event->isAutoRepeat() ) {
        emit gotNoteOffEvent(NoteOffEvent(0, 0, offset+idx, 0));
    } else {
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::on_actionBubbleView_triggered() {
    setCentralWidget(bubbleView);
}

void MainWindow::on_actionGuidoView_triggered() {
    setCentralWidget(guidoView);
}

void MainWindow::on_actionFull_Screen_triggered() {
    static int state;
    if (isFullScreen()) {
        if (state & Qt::WindowMaximized) {
            showMaximized();
        } else if ( state & Qt::WindowMinimized ) {
            showMinimized();
        } else {
            showNormal();
        }
    } else {
        state = windowState();
        showFullScreen();
    }
}

void MainWindow::on_actionSettings_triggered() {
    SettingsDialog* settingsDialog = new SettingsDialog(this);
    settingsDialog->init(midiWrapper);
    settingsDialog->exec();
}
