#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <ctime>

#include <QDir>
#include <QFileDialog>
#include <QSettings>

#include "bubbleview.h"
#include "chordwidget.h"
#include "feedbackdialog.h"
#include "guidoview.h"
#include "settingsdialog.h"
#include "timelineview.h"

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

    midiClusterHandler = new ClusterHandler();
    dummyClusterHandler = new ClusterHandler();
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
    QObject::connect(this, &MainWindow::gotNoteOnEvent, dummyClusterHandler, &ClusterHandler::matchNoteOnEvent);
    QObject::connect(this, &MainWindow::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);
    QObject::connect(this, &MainWindow::gotNoteOffEvent, dummyClusterHandler, &ClusterHandler::matchNoteOffEvent);

    // Core connections
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, midiClusterHandler, &ClusterHandler::matchNoteOnEvent);
    QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, midiClusterHandler, &ClusterHandler::matchNoteOffEvent);
    QObject::connect(midiClusterHandler, &ClusterHandler::gotMidiPairClusters, matchingHandler, &MatchingHandler::matchMidiPairClusters);
    QObject::connect(midiClusterHandler, &ClusterHandler::reset, matchingHandler, &MatchingHandler::reset);
    QObject::connect(midiClusterHandler, &ClusterHandler::reset, signalManager, &SignalManager::playResetSound);

    QObject::connect(matchingHandler, &MatchingHandler::songRecognized, mergingHandler, &MergingHandler::eatMatchingItem);
    QObject::connect(matchingHandler, &MatchingHandler::songFinished, signalManager, &SignalManager::playFinishedSound);
    QObject::connect(matchingHandler, &MatchingHandler::songFinished, resultManager, &ResultManager::saveFinishedSong);

    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);
    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOnEvent, dummyClusterHandler, &ClusterHandler::matchNoteOnEvent);
    QObject::connect(playbackHandler, &PlaybackHandler::gotNoteOffEvent, dummyClusterHandler, &ClusterHandler::matchNoteOffEvent);

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

    delete midiClusterHandler;
    delete dummyClusterHandler;
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
        NoteOnEvent noteOn = NoteOnEvent(time(NULL), 0, offset+idx, 50);
        emit gotNoteOnEvent(noteOn);
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (GuidoView *gv = qobject_cast<GuidoView*>(centralWidget())) {
        if (event->key() == Qt::Key_Left) {
            emit previousPage();
        } else if (event->key() == Qt::Key_Right) {
            emit nextPage();
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
        NoteOffEvent noteOff = NoteOffEvent(time(NULL), 0, offset+idx, 0);
        emit gotNoteOffEvent(noteOff);
    } else {
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::on_actionBubbleView_triggered() {
    bubbleView = new BubbleView();

    QObject::connect(midiClusterHandler, &ClusterHandler::gotMidiPairClusters, bubbleView, &BubbleView::showMidiPairClusters);
    QObject::connect(midiClusterHandler, &ClusterHandler::reset, bubbleView, &BubbleView::reset);
    QObject::connect(dummyClusterHandler, &ClusterHandler::gotMidiPairClusters, bubbleView, &BubbleView::showMidiPairClusters);
    QObject::connect(dummyClusterHandler, &ClusterHandler::reset, bubbleView, &BubbleView::reset);

    QObject::connect(bubbleView, &BubbleView::gotNoteOnEvent, midiWrapper, &MidiWrapper::playNoteOnEvent);
    QObject::connect(bubbleView, &BubbleView::gotNoteOffEvent, midiWrapper, &MidiWrapper::playNoteOffEvent);

    setCentralWidget(bubbleView);
}

void MainWindow::on_actionGuidoView_triggered() {
    guidoView = new GuidoView();

    QString scoreDirectory = QString("%1/meisterschueler-misc/scores").arg(MAINPATH);
    QList<Song> songs = SongService::getSongsFromDirectory(scoreDirectory);
    guidoView->setSongs(songs);

    QObject::connect(this, &MainWindow::previousPage, guidoView, &GuidoView::previousPage);
    QObject::connect(this, &MainWindow::nextPage, guidoView, &GuidoView::nextPage);
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
    settingsDialog->init(midiWrapper, resultManager);
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

    QObject::connect(midiClusterHandler, &ClusterHandler::gotMidiPairClusters, customView, &CustomView::showMidiPairClusters);

    setCentralWidget(customView);
}

void MainWindow::on_actionTimelineView_triggered()
{
    TimelineView *timelineView = new TimelineView();

    QObject::connect(midiClusterHandler, &ClusterHandler::gotMidiPairCluster, timelineView, &TimelineView::showMidiPairCluster);
    //QObject::connect(midiWrapper, &MidiWrapper::gotNoteOnEvent, timelineView, &TimelineView::showNoteOnEvent);
    //QObject::connect(midiWrapper, &MidiWrapper::gotNoteOffEvent, timelineView, &TimelineView::showNoteOffEvent);

    setCentralWidget(timelineView);
}
