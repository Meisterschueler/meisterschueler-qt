#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class MidiWrapper;
class ResultManager;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void init(MidiWrapper *midiWrapper, ResultManager *resultManager);
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void on_toolButtonResultFilePath_clicked();

    void on_toolButtonCustomScoresPath_clicked();

private:
    Ui::SettingsDialog *ui;

    MidiWrapper *midiWrapper;
    ResultManager *resultManager;
};

#endif // SETTINGSDIALOG_H
