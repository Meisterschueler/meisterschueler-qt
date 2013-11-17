#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QSettings>

#include "bubbleview.h"
#include "chordwidget.h"
#include "feedbackdialog.h"
#include "guidoview.h"
#include "settingsdialog.h"

#include "clusterhandler.h"
#include "commandmanager.h"
#include "customview.h"
#include "feedbackmanager.h"
#include "matchinghandler.h"
#include "merginghandler.h"
#include "midiservice.h"
#include "midiwrapper.h"
#include "playbackhandler.h"
#include "recordhandler.h"
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

    clusterHandler = new ClusterHandler();
    commandManager = new CommandManager();
    feedbackManager = new FeedbackManager();
    midiWrapper = new MidiWrapper();
    QList<Song> songs = SongService::getSongsBuiltIn();
    QList<MatchingItem> matchingItems = SongService::createMatchingItems(songs);
    matchingHandler = new MatchingHandler(matchingItems);
    mergingHandler = new MergingHandler();
    playbackHandler = new PlaybackHandler();
    recordHandler = new RecordHandler();
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
    if (false) {
        QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, matchingHandler, &MatchingHandler::matchNoteOnEvent);
        QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, matchingHandler, &MatchingHandler::matchNoteOffEvent);
    } else {
        QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, clusterHandler, &ClusterHandler::matchNoteOnEvent);
        QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, clusterHandler, &ClusterHandler::matchNoteOffEvent);
        QObject::connect(clusterHandler, &ClusterHandler::gotChannelEvents, matchingHandler, &MatchingHandler::matchChannelEvents);
        QObject::connect(clusterHandler, &ClusterHandler::reset, matchingHandler, &MatchingHandler::reset);
        QObject::connect(clusterHandler, &ClusterHandler::reset, signalManager, &SignalManager::playResetSound);
    }

    QObject::connect(matchingHandler, &MatchingHandler::songRecognized, mergingHandler, &MergingHandler::eatMatchingItem);
    QObject::connect(matchingHandler, &MatchingHandler::songFinished, signalManager, &SignalManager::playFinishedSound);
    QObject::connect(matchingHandler, &MatchingHandler::songFinished, resultManager, &ResultManager::saveFinishedSong);

    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    QObject::connect(signalManager, &SignalManager::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(signalManager, &SignalManager::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    // EchoManager
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, feedbackManager, &FeedbackManager::playNoteOnEvent);
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, feedbackManager, &FeedbackManager::playNoteOffEvent);

    QObject::connect(feedbackManager, &FeedbackManager::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(feedbackManager, &FeedbackManager::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    // CommandManager
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, commandManager, &CommandManager::handleNoteOnEvent);
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, commandManager, &CommandManager::handleNoteOffEvent);
    QObject::connect(midiWrapper, &MidiWrapper::gotControlChangeEvent, commandManager, &CommandManager::handleControlChangeEvent);

    QObject::connect(commandManager, &CommandManager::startRecording, recordHandler, &RecordHandler::startRecording);
    QObject::connect(commandManager, &CommandManager::startRecording, signalManager, &SignalManager::playRecordStartSignal);

    QObject::connect(commandManager, &CommandManager::stopRecording, recordHandler, &RecordHandler::stopRecording);
    QObject::connect(commandManager, &CommandManager::stopRecording, signalManager, &SignalManager::playRecordStopSignal);

    on_actionBubbleView_triggered();

    signalManager->playStartupSound();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete clusterHandler;
    delete commandManager;
    delete feedbackManager;
    delete midiWrapper;
    delete matchingHandler;
    delete mergingHandler;
    delete playbackHandler;
    delete recordHandler;
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
        NoteOnEvent noteOn = NoteOnEvent(0, 0, offset+idx, 50);
        clusterHandler->matchNoteOnEvent(noteOn);
        emit gotNoteOnEvent(noteOn);
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (GuidoView *gv = qobject_cast<GuidoView*>(centralWidget())) {
        if (event->key() == Qt::Key_Left) {
            gv->previousPage();
        } else if (event->key() == Qt::Key_Right) {
            gv->nextPage();
        }
    }

    int idx = keys.indexOf((Qt::Key)event->key());
    int offset = -1;
    if (event->modifiers() == Qt::NoModifier) {
        offset = 48;
    } else if (event->modifiers() == Qt::ShiftModifier) {
        offset = 60;
    }

    if ( idx != -1 && !event->isAutoRepeat() ) {
        NoteOffEvent noteOff = NoteOffEvent(0, 0, offset+idx, 0);
        clusterHandler->matchNoteOffEvent(noteOff);
        emit gotNoteOffEvent(noteOff);
    } else {
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::on_actionBubbleView_triggered() {
    bubbleView = new BubbleView();

    QObject::connect(clusterHandler, &ClusterHandler::gotMidiPairClusters, bubbleView, &BubbleView::showMidiPairClusters);
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

void MainWindow::on_actionFeedback_triggered()
{
    FeedbackDialog *feedbackDialog = new FeedbackDialog(this);
    feedbackDialog->init(feedbackManager);
    feedbackDialog->exec();
}

void MainWindow::on_actionChordWidget_triggered()
{
    ChordWidget *chordWidget = new ChordWidget();

    setCentralWidget(chordWidget);
}

void MainWindow::on_actionExit_triggered()
{
    signalManager->playShutdownSound();
    exit(0);
}


void MainWindow::on_actionActionQCustomPlot_triggered()
{
    CustomView *customView = new CustomView();

    QObject::connect(clusterHandler, &ClusterHandler::gotMidiPairClusters, customView, &CustomView::showMidiPairClusters);

    setCentralWidget(customView);
}
