#include "feedbackdialog.h"
#include "ui_feedbackdialog.h"

#include "feedbackmanager.h"

FeedbackDialog::FeedbackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeedbackDialog)
{
    ui->setupUi(this);
}

FeedbackDialog::~FeedbackDialog()
{
    delete ui;
}

void FeedbackDialog::init(FeedbackManager *echoManager) {
    this->feedbackManager=echoManager;

    switch (echoManager->getState()) {
    case FeedbackManager::OFF:
        ui->radioButtonOff->toggle();
        ui->horizontalSliderDelay->setEnabled(false);
        ui->horizontalSliderVelocity->setEnabled(false);
        ui->horizontalSliderHurdle->setEnabled(false);
        break;
    case FeedbackManager::ECHO:
        ui->radioButtonEcho->toggle();
        ui->horizontalSliderDelay->setEnabled(true);
        ui->horizontalSliderVelocity->setEnabled(false);
        ui->horizontalSliderHurdle->setEnabled(false);
        break;
    case FeedbackManager::REPING:
        ui->radioButtonReping->toggle();
        ui->horizontalSliderDelay->setEnabled(false);
        ui->horizontalSliderVelocity->setEnabled(true);
        ui->horizontalSliderHurdle->setEnabled(false);
        break;
    case FeedbackManager::HURDLE:
        ui->radioButtonHurdle->toggle();
        ui->horizontalSliderDelay->setEnabled(false);
        ui->horizontalSliderVelocity->setEnabled(false);
        ui->horizontalSliderHurdle->setEnabled(true);
        break;
    }

    ui->horizontalSliderDelay->setValue(echoManager->getEchoDelay());
    ui->horizontalSliderHurdle->setValue(echoManager->getHurdleVelocity());

    QObject::connect(ui->radioButtonOff, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleOff);
    QObject::connect(ui->radioButtonEcho, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleEcho);
    QObject::connect(ui->radioButtonReping, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleReping);
    QObject::connect(ui->radioButtonHurdle, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleHurdle);

    QObject::connect(ui->horizontalSliderDelay, &QSlider::valueChanged, echoManager, &FeedbackManager::setEchoDelay);
    QObject::connect(ui->horizontalSliderHurdle, &QSlider::valueChanged, echoManager, &FeedbackManager::setHurdleVelocity);
}

void FeedbackDialog::changeEvent(QEvent *e)
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
