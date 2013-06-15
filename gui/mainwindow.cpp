#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QSettings>

#include "bubbleview.h"
#include "echodialog.h"
#include "guidoview.h"
#include "settingsdialog.h"

#include "echomanager.h"
#include "matchinghandler.h"
#include "merginghandler.h"
#include "midiservice.h"
#include "midiwrapper.h"
#include "playbackhandler.h"
#include "resultmanager.h"
#include "signalmanager.h"
#include "songservice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<NoteOnEvent>("NoteOnEvent");
    qRegisterMetaType<NoteOffEvent>("NoteOffEvent");

    echoManager = new EchoManager();
    midiWrapper = new MidiWrapper();
    QList<Song> songs = SongService::getSongsBuiltIn();
    QList<MatchingItem> matchingItems = SongService::createMatchingItems(songs);
    matchingHandler = new MatchingHandler(matchingItems);
    mergingHandler = new MergingHandler();
    playbackHandler = new PlaybackHandler();
    resultManager = new ResultManager();
    signalManager = new SignalManager();

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    // Pure GUI connections
    QObject::connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // GUI/Core connections
    QObject::connect(this, &MainWindow::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(this, &MainWindow::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    // Core connections
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, matchingHandler, &MatchingHandler::matchNoteOnEvent);
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, matchingHandler, &MatchingHandler::matchNoteOffEvent);

    QObject::connect(matchingHandler, &MatchingHandler::songRecognized, mergingHandler, &MergingHandler::eatMatchingItem);
    QObject::connect(matchingHandler, &MatchingHandler::songFinished, signalManager, &SignalManager::playFinishedSound);
    QObject::connect(matchingHandler, &MatchingHandler::songFinished, resultManager, &ResultManager::analyseFinishedSong);

    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    QObject::connect(signalManager, &SignalManager::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(signalManager, &SignalManager::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    // EchoManager
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, echoManager, &EchoManager::playNoteOnEvent);
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, echoManager, &EchoManager::playNoteOffEvent);

    QObject::connect(echoManager, &EchoManager::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(echoManager, &EchoManager::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    on_actionBubbleView_triggered();

    signalManager->playStartupSound();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete echoManager;
    delete midiWrapper;
    delete matchingHandler;
    delete mergingHandler;
    delete playbackHandler;
    delete resultManager;
    delete signalManager;
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
    bubbleView = new BubbleView();

    QObject::connect(this, &MainWindow::gotNoteOnEvent, bubbleView, &BubbleView::showNoteOnEvent);
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, bubbleView, &BubbleView::showNoteOnEvent);
    QObject::connect(bubbleView, &BubbleView::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(bubbleView, &BubbleView::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    setCentralWidget(bubbleView);
}

void MainWindow::on_actionGuidoView_triggered() {
    guidoView = new GuidoView();

    QObject::connect(guidoView, &GuidoView::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(guidoView, &GuidoView::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

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

void MainWindow::on_actionLoad_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load MIDI file"), QDir::homePath(), tr("MIDI files (*.mid *.midi)"));
    if (filename.isEmpty())
        return;

    QList<ChannelEvent> channelEvents = MidiService::load(filename);
    playbackHandler->setChannelEvents(channelEvents);
    playbackHandler->play();
}

void MainWindow::on_actionEcho_triggered()
{
    EchoDialog *echoDialog = new EchoDialog(this);
    echoDialog->init(echoManager);
    echoDialog->exec();
}
