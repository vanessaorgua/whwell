#include "dlgshibctrl.h"
#include "ui_dlgshibctrl.h"
#include "iodev.h"

#include <QDebug>

DlgShibCtrl::DlgShibCtrl(IoDev &src,QWidget *parent) :
    QDialog(parent),
    s(src),
    ui(new Ui::DlgShibCtrl)
{
    ui->setupUi(this);
    ui->bx_AM_01->setCurrentIndex(s.getValue16("AM_01")?1:0);
    ui->bx_Y_01->setCurrentIndex(s.getValue16("Y_01")?1:0);

    connect(ui->bx_AM_01,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetValue(int)));
    connect(ui->bx_Y_01,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetValue(int)));

   ui->bx_Y_01->blockSignals(s.getValue16("AM_01"));
}

DlgShibCtrl::~DlgShibCtrl()
{
    delete ui;
}

void DlgShibCtrl::changeEvent(QEvent *e)
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

void DlgShibCtrl::slotSetValue(int v)
{
    qDebug() << "SlotSendValue" << v << " to " << sender()->objectName().right(sender()->objectName().size()-3);
    s.sendValue(sender()->objectName().right(sender()->objectName().size()-3),qint16(-v));
    if(sender()->objectName()=="bx_AM_01")
        ui->bx_Y_01->blockSignals(v);

}

