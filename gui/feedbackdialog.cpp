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
        ui->horizontalSliderTunnelMin->setEnabled(false);
        ui->horizontalSliderTunnelMax->setEnabled(false);
        break;
    case FeedbackManager::ECHO:
        ui->radioButtonEcho->toggle();
        ui->horizontalSliderDelay->setEnabled(true);
        ui->horizontalSliderVelocity->setEnabled(false);
        ui->horizontalSliderTunnelMin->setEnabled(false);
        ui->horizontalSliderTunnelMax->setEnabled(false);
        break;
    case FeedbackManager::REPING:
        ui->radioButtonReping->toggle();
        ui->horizontalSliderDelay->setEnabled(false);
        ui->horizontalSliderVelocity->setEnabled(true);
        ui->horizontalSliderTunnelMin->setEnabled(false);
        ui->horizontalSliderTunnelMax->setEnabled(false);
        break;
    case FeedbackManager::TUNNEL:
        ui->radioButtonTunnel->toggle();
        ui->horizontalSliderDelay->setEnabled(false);
        ui->horizontalSliderVelocity->setEnabled(false);
        ui->horizontalSliderTunnelMin->setEnabled(true);
        ui->horizontalSliderTunnelMax->setEnabled(true);
        break;
    }

    ui->horizontalSliderDelay->setValue(echoManager->getEchoDelay());
    ui->horizontalSliderTunnelMin->setValue(echoManager->getTunnelMin());
    ui->horizontalSliderTunnelMax->setValue(echoManager->getTunnelMax());

    QObject::connect(ui->radioButtonOff, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleOff);
    QObject::connect(ui->radioButtonEcho, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleEcho);
    QObject::connect(ui->radioButtonReping, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleReping);
    QObject::connect(ui->radioButtonTunnel, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleTunnel);

    QObject::connect(ui->horizontalSliderDelay, &QSlider::valueChanged, echoManager, &FeedbackManager::setEchoDelay);
    QObject::connect(ui->horizontalSliderTunnelMin, &QSlider::valueChanged, echoManager, &FeedbackManager::setTunnelMin);
    QObject::connect(ui->horizontalSliderTunnelMax, &QSlider::valueChanged, echoManager, &FeedbackManager::setTunnelMax);
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
