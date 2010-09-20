#include "alertviewdialog.h"
#include "ui_alertviewdialog.h"

AlertViewDialog::AlertViewDialog(QStack<QString> v, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AlertViewDialog)
{
    m_ui->setupUi(this);

    for(; !v.isEmpty();)
    {
        m_ui->AlertViewr->append(v.pop());
    }

}

AlertViewDialog::~AlertViewDialog()
{
    delete m_ui;
}

void AlertViewDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AlertViewDialog::updateAlert(QString)
{

}
