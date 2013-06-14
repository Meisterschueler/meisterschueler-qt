#include "echodialog.h"
#include "ui_echodialog.h"

#include "echomanager.h"

EchoDialog::EchoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EchoesDialog)
{
    ui->setupUi(this);
}

EchoDialog::~EchoDialog()
{
    delete ui;
}

void EchoDialog::init(EchoManager *echoManager) {
    this->echoManager=echoManager;

    switch (echoManager->getState()) {
    case EchoManager::OFF:
        ui->radioButtonOff->toggle();
        ui->horizontalSliderDelay->setEnabled(false);
        ui->horizontalSliderVelocity->setEnabled(false);
        break;
    case EchoManager::ECHO:
        ui->radioButtonEcho->toggle();
        ui->horizontalSliderDelay->setEnabled(true);
        ui->horizontalSliderVelocity->setEnabled(false);
        break;
    case EchoManager::REPING:
        ui->radioButtonReping->toggle();
        ui->horizontalSliderDelay->setEnabled(false);
        ui->horizontalSliderVelocity->setEnabled(true);
        break;
    }

    ui->horizontalSliderDelay->setValue(echoManager->getEchoDelay());

    QObject::connect(ui->radioButtonOff, &QRadioButton::toggled, echoManager, &EchoManager::toggleOff);
    QObject::connect(ui->radioButtonEcho, &QRadioButton::toggled, echoManager, &EchoManager::toggleEcho);
    QObject::connect(ui->radioButtonReping, &QRadioButton::toggled, echoManager, &EchoManager::toggleReping);

    QObject::connect(ui->horizontalSliderDelay, &QSlider::valueChanged, echoManager, &EchoManager::setEchoDelay);
}

void EchoDialog::changeEvent(QEvent *e)
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
