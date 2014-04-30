#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H

#include <QDialog>

namespace Ui {
class FeedbackDialog;
}

class FeedbackManager;

class FeedbackDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FeedbackDialog(QWidget *parent = 0);
    ~FeedbackDialog();

    void init(FeedbackManager *feedbackManager);
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_horizontalSliderTunnelMin_sliderMoved(int position);

    void on_horizontalSliderTunnelMax_sliderMoved(int position);

    void on_horizontalSliderEchoesDelay_sliderMoved(int position);

    void on_horizontalSliderRePingVelocity_sliderMoved(int position);

private:
    Ui::FeedbackDialog *ui;

    FeedbackManager *feedbackManager;
};

#endif // FEEDBACKDIALOG_H
