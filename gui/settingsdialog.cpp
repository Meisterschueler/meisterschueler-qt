#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>

#include "../src/midiwrapper.h"
#include "../src/resultmanager.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::init(MidiWrapper *midiWrapper, ResultManager *resultManager) {
    this->midiWrapper = midiWrapper;

    // MIDI tab
    QStringList inputPorts = midiWrapper->getInputPorts();
    ui->comboBoxInputDevice->addItems(inputPorts);
    ui->comboBoxInputDevice->setCurrentIndex(inputPorts.indexOf(midiWrapper->getOpenedInputPort()));
    QObject::connect(ui->comboBoxInputDevice, SIGNAL(currentIndexChanged(QString)), midiWrapper, SLOT(openInputPort(QString)));

    QStringList outputPorts = midiWrapper->getOutputPorts();
    ui->comboBoxOutputDevice->addItems(outputPorts);
    ui->comboBoxOutputDevice->setCurrentIndex(outputPorts.indexOf(midiWrapper->getOpenedOutputPort()));
    QObject::connect(ui->comboBoxOutputDevice, SIGNAL(currentIndexChanged(QString)), midiWrapper, SLOT(openOutputPort(QString)));

    // Path tab
    QString resultFilePath = resultManager->getResultFilePath();
    ui->labelResultFilesPath->setText(resultFilePath);

    ui->labelCustomScoresPath->setText(""); // TODO: hier muss der Custom scores path rein...
}

void SettingsDialog::on_toolButtonResultFilePath_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, "Result File Path");
    if (!fileName.isEmpty()) {
        ui->labelResultFilesPath->setText(fileName);
        resultManager->setResultFilePath(fileName);
    }
}

void SettingsDialog::on_toolButtonCustomScoresPath_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, "Custom Scores Path");
    if (!fileName.isEmpty()) {
        ui->labelCustomScoresPath->setText(fileName);
    }
}
