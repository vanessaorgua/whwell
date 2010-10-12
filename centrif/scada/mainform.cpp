#include "mainform.h"

#include "mnemo.h"
#include "one_mnemo.h"

#include <QDebug>

#include <QMessageBox>
#include <IoNetClient.h>

#include "sparamdialog.h"
#include "about.h"
#include "history.h"
#include "alertviewdialog.h"

#include "cfparam.h"
#include "report.h"




mMainForm::mMainForm(IoNetClient &source,QWidget *p): QWidget(p),
        src(source),
        m_ui(new Ui::Form),
        mMnemo(new Mnemo(source,this))

{

    m_ui->setupUi(this);
    

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->setObjectName("Clock_timer");
    
    connect(t,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(m_ui->About,SIGNAL(clicked()),this,SLOT(about()));
    connect(m_ui->Trend,SIGNAL(clicked()),this,SLOT(trRun()));
    connect(m_ui->bnParm,SIGNAL(clicked()),this,SLOT(setupParm()));

    connect (m_ui->Exit,SIGNAL(clicked()),this,SLOT(slotExit()));

    connect(m_ui->bn_showAlert,SIGNAL(clicked()),this,SLOT(showAlert()));
    connect(m_ui->bnReport,SIGNAL(clicked()),this,SLOT(showReport()));
    connect(m_ui->bnShowMain,SIGNAL(clicked()),this,SLOT(showMainMnemo()));
    m_ui->bnShowMain->hide(); // приховати тимчасово

    // відобразити мнемосхему
    m_ui->sW->addWidget(mMnemo);

    // відображення свіжих алертів
    connect(&src,SIGNAL(Alert(QString)),this,SLOT(slotAlert(QString)));

    
    t->start();


}

mMainForm::~mMainForm()
{
    delete m_ui;
}


void mMainForm::timeOut()
{
    QDateTime tm;
    

    tm=QDateTime::currentDateTime();
    
    m_ui->lineEDate->setText(tm.toString("dd:MM:yyyy"));
    m_ui->lineETime->setText(tm.toString("hh:mm:ss"));
    
}

void mMainForm::about()
{
    aboutDlg *ab=new aboutDlg(this);
    ab->exec();
    delete ab;
}


void mMainForm::trRun()
{

    int nHeight=4000;

//    qDebug() << sender()->objectName();
    RHistorySelect *trd = new RHistorySelect(src,&tp,this);

    if(trd->exec()==QDialog::Accepted)
    {

	pTrw = new TrendWindow(NULL,&tp,nHeight);
        if(tp.trend!="trend0trend5")
        {
            QVector<QColor> clr;
            clr << Qt::green << Qt::red << Qt::yellow <<QColor(236,158,94) << Qt::cyan <<Qt::darkMagenta << Qt::darkYellow << Qt::darkGreen;
            pTrw->setColors(clr);
        }
        //pTrw->setAttribute( Qt::WA_DeleteOnClose);

        connect(pTrw,SIGNAL(finished()),this,SLOT(showMe()));

	((QStackedWidget*)parentWidget())->addWidget(pTrw);       // показати вікно графіків
        ((QStackedWidget*)parentWidget())->setCurrentWidget(pTrw);
    }
    delete trd;
    //qDebug() << "End Init";
}

void mMainForm::showMe()
{
    ((QStackedWidget*)parentWidget())->setCurrentWidget(this) ;
    ((QStackedWidget*)parentWidget())->removeWidget(pTrw);
    delete pTrw;
}

void mMainForm::setupParm()
{
    if(m_ui->sW->currentIndex()>0)
    {

      CfParam p(*src[ oM->currentCfNom()],this);
      p.setWindowTitle(QString(tr("Параметри  цф№%1").arg(oM->currentCfNom()+1)));
      p.exec();
    }
    else
    {
        SParamDialog d(src,this);
        d.exec();
    }
}

void mMainForm::slotExit()
{
    if(QMessageBox::warning(this,tr("Попередження"),tr("Роботу програми буде завершено\nПродовжити?"),
	QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes)
    {
        qobject_cast<QWidget*>(parent())->close() ; // тоді закритися
    }
}

void mMainForm::slotAlert(QString v)
{
    QString a=QString("%1 | %2").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")).arg(v);
    m_ui->AlertBrowser->clear();
    m_ui->AlertBrowser->insertPlainText( a );


    alertList.push(a);          // зберегти поточний
    for(;alertList.size()>100;) // якщо більше 100
    {
        alertList.pop_front();  // видалити зайві
    }
}

void mMainForm::showAlert()
{
    AlertViewDialog d(alertList,this);
    d.exec();
}


void mMainForm::showOneMnemo() // ця функція слугє для виводу мнемосхеми одиночної центрифуги
{
    int i=sender()->objectName().right(1).toInt()-1;
    m_ui->bnShowMain->show(); // показати кнопку повернення назад

    currentIndex=0; //
    oM =new OneMnemo(src,mMnemo->getTrChart(),i,this);
    m_ui->sW->addWidget(oM);
    m_ui->sW->setCurrentWidget(oM);

}

void mMainForm::showMainMnemo() // ця функція поверне головну мнемосхему
{
    m_ui->bnShowMain->hide(); // показати кнопку повернення назад
    m_ui->sW->setCurrentIndex(0);// показати головну мнемосхему
    if(currentIndex) // 1 - показується звіт
    {
        m_ui->bnReport->blockSignals(false);
        m_ui->sW->removeWidget(rep);
        delete rep;
    }
    else // 0 - показується мнемосхема одиночної центрифуги
    {
        m_ui->sW->removeWidget(oM);
        delete oM; //
    }

}

void mMainForm::showReport() // показати сторінку звітів
{

    if(m_ui->bnShowMain->isHidden()) // якщо кнопка повернення схована тоді
    {
        m_ui->bnShowMain->show(); // показати кнопку повернення
    }
    else // якщо кнопка вже показана
    { // вивантажити мнемосхему центрифуг
        m_ui->sW->setCurrentIndex(0);
        m_ui->sW->removeWidget(oM);
        delete oM;
    }
    // створити та відобразити сторінку звітів... тможе треба б додати менеджер розміщення
    rep=new  Report;
    m_ui->sW->addWidget(rep);
    m_ui->sW->setCurrentWidget(rep);
    currentIndex=1;

    sender()->blockSignals(true); //заблокувати повторну відправку сигналу

}




