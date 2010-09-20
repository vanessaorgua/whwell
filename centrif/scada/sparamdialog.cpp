#include "sparamdialog.h"
#include "ui_sparamdialog.h"
#include <IoNetClient.h>
#include <QSettings>

#include <QDebug>


SParamDialog::SParamDialog(IoNetClient &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SParamDialog),
    s(src),ch(-1)
{
    m_ui->setupUi(this);


    connect(m_ui->bn_Ok,SIGNAL(clicked()),this,SLOT(myAccept()));

    QSettings set;
    m_ui->le_ioserv->setText(set.value("/ioserv/hostname","localhost").toString());

    m_ui->le_dbserv->setText(set.value("/db/hostname","localhost").toString());
    m_ui->le_dbname->setText(set.value("/db/dbname","lynovycya").toString());
    m_ui->le_username->setText(set.value("/db/username","scada").toString());
    m_ui->le_passwd->setText(set.value("/db/passwd","").toString());


    m_ui->Psir_min->setValue(s[0]->getValue16("Psir_min"));
    m_ui->Tvod_zd->setValue(s[0]->getValue16("Tvod_zd"));
}

SParamDialog::~SParamDialog()
{
    delete m_ui;
}

void SParamDialog::changeEvent(QEvent *e)
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


void SParamDialog::myAccept()
{
    QSettings set;
    set.setValue("/ioserv/hostname",m_ui->le_ioserv->text());

    set.setValue("/db/hostname",m_ui->le_dbserv->text());
    set.setValue("/db/dbname",m_ui->le_dbname->text());
    set.setValue("/db/username",m_ui->le_username->text());
    set.setValue("/db/passwd",m_ui->le_passwd->text());

    for(int i=0;i<5;++i)
    {
        s[i]->sendValue("Psir_min",qint16(m_ui->Psir_min->value()));
        s[i]->sendValue("Tvod_zd",qint16(m_ui->Tvod_zd->value()));
    }

    accept();
}



void SParamDialog::slotSet(QString )
{
    //qDebug() << sender()->objectName() << v;
}

void SParamDialog::selectTeg(int v)
{
    // відправити змінені дані
}



