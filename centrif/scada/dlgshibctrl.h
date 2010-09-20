#ifndef DLGSHIBCTRL_H
#define DLGSHIBCTRL_H

#include <QDialog>

class IoDev;

namespace Ui {
    class DlgShibCtrl;
}

class DlgShibCtrl : public QDialog {
    Q_OBJECT
public:
    DlgShibCtrl(IoDev &src, QWidget *parent = 0);
    ~DlgShibCtrl();
public slots:
    void slotSetValue(int);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DlgShibCtrl *ui;
    IoDev &s;
};

#endif // DLGSHIBCTRL_H
