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

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    QObject::connect(ui->actionFull_Screen, SIGNAL(triggered()), this, SLOT(toggleFullscreen()));
    QObject::connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    bubbleView = new BubbleView();
    setCentralWidget(bubbleView);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete midiWrapper;
    delete bubbleView;
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

void MainWindow::toggleFullscreen() {
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

void MainWindow::showSettingsDialog() {
    SettingsDialog* settingsDialog = new SettingsDialog(this);
    settingsDialog->init(midiWrapper);
    settingsDialog->exec();
}
