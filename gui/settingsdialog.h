#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "../src/midiwrapper.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void init(MidiWrapper *midiWrapper);
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::SettingsDialog *ui;

    MidiWrapper *midiWrapper;
};

#endif // SETTINGSDIALOG_H
