#ifndef GUIDOVIEW_H
#define GUIDOVIEW_H

#include <QWidget>

namespace Ui {
class GuidoView;
}

class GuidoView : public QWidget
{
    Q_OBJECT
    
public:
    explicit GuidoView(QWidget *parent = 0);
    ~GuidoView();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::GuidoView *ui;
};

#endif // GUIDOVIEW_H
