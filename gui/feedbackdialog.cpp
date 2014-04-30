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
        ui->horizontalSliderEchoesDelay->setEnabled(false);
        ui->horizontalSliderRePingVelocity->setEnabled(false);
        ui->horizontalSliderTunnelMin->setEnabled(false);
        ui->horizontalSliderTunnelMax->setEnabled(false);
        break;
    case FeedbackManager::ECHO:
        ui->radioButtonEcho->toggle();
        ui->horizontalSliderEchoesDelay->setEnabled(true);
        ui->horizontalSliderRePingVelocity->setEnabled(false);
        ui->horizontalSliderTunnelMin->setEnabled(false);
        ui->horizontalSliderTunnelMax->setEnabled(false);
        break;
    case FeedbackManager::REPING:
        ui->radioButtonReping->toggle();
        ui->horizontalSliderEchoesDelay->setEnabled(false);
        ui->horizontalSliderRePingVelocity->setEnabled(true);
        ui->horizontalSliderTunnelMin->setEnabled(false);
        ui->horizontalSliderTunnelMax->setEnabled(false);
        break;
    case FeedbackManager::TUNNEL:
        ui->radioButtonTunnel->toggle();
        ui->horizontalSliderEchoesDelay->setEnabled(false);
        ui->horizontalSliderRePingVelocity->setEnabled(false);
        ui->horizontalSliderTunnelMin->setEnabled(true);
        ui->horizontalSliderTunnelMax->setEnabled(true);
        break;
    case FeedbackManager::PINGPONG:
        ui->radioButtonPingPong->toggle();
        ui->horizontalSliderEchoesDelay->setEnabled(false);
        ui->horizontalSliderRePingVelocity->setEnabled(false);
        ui->horizontalSliderTunnelMin->setEnabled(false);
        ui->horizontalSliderTunnelMax->setEnabled(false);
        break;
    }

    ui->horizontalSliderEchoesDelay->setValue(echoManager->getEchoDelay());
    ui->horizontalSliderTunnelMin->setValue(echoManager->getTunnelMin());
    ui->horizontalSliderTunnelMax->setValue(echoManager->getTunnelMax());

    QObject::connect(ui->radioButtonOff, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleOff);
    QObject::connect(ui->radioButtonEcho, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleEcho);
    QObject::connect(ui->radioButtonReping, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleReping);
    QObject::connect(ui->radioButtonTunnel, &QRadioButton::toggled, echoManager, &FeedbackManager::toggleTunnel);
    QObject::connect(ui->radioButtonPingPong, &QRadioButton::toggled, echoManager, &FeedbackManager::togglePingPong);

    QObject::connect(ui->horizontalSliderEchoesDelay, &QSlider::valueChanged, echoManager, &FeedbackManager::setEchoDelay);
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

void FeedbackDialog::on_horizontalSliderEchoesDelay_sliderMoved(int position)
{
    ui->labelEchoesDelay->setText(tr("%1ms").arg(position));
}

void FeedbackDialog::on_horizontalSliderRePingVelocity_sliderMoved(int position)
{
    ui->labelRePingVelocity->setText(tr("%1").arg(position));
}

void FeedbackDialog::on_horizontalSliderTunnelMin_sliderMoved(int position)
{
    ui->labelTunnelMin->setText(tr("%1").arg(position));
}

void FeedbackDialog::on_horizontalSliderTunnelMax_sliderMoved(int position)
{
    ui->labelTunnelMax->setText(tr("%1").arg(position));
}


