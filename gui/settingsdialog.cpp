#include "settingsdialog.h"
#include "ui_settingsdialog.h"

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

void SettingsDialog::init(MidiWrapper *midiWrapper) {
    this->midiWrapper = midiWrapper;

    QStringList inputPorts = midiWrapper->getInputPorts();
    ui->comboBoxInputDevice->addItem(tr("(none)"));
    ui->comboBoxInputDevice->addItems(inputPorts);
    ui->comboBoxInputDevice->setCurrentIndex(inputPorts.indexOf(midiWrapper->getOpenedInputPort()));
    QObject::connect(ui->comboBoxInputDevice, SIGNAL(currentIndexChanged(QString)), midiWrapper, SLOT(openInputPort(QString)));
}
