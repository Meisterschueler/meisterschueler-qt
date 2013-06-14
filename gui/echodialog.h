#ifndef ECHOESDIALOG_H
#define ECHOESDIALOG_H

#include <QDialog>

namespace Ui {
class EchoesDialog;
}

class EchoManager;

class EchoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EchoDialog(QWidget *parent = 0);
    ~EchoDialog();

    void init(EchoManager *echoManager);
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::EchoesDialog *ui;

    EchoManager *echoManager;
};

#endif // ECHOESDIALOG_H
