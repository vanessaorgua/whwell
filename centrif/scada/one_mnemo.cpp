
#include "one_mnemo.h"
#include "ui_one_mnemo.h"
#include <trendchart.h>

#include <IoNetClient.h>

#include <QVBoxLayout>


OneMnemo::OneMnemo(IoNetClient &src,QVector<TrendChart*> &masterTrChart, int cfNom, QWidget *p) :
        QLabel(p),
        m_ui(new Ui::one_mnemo),
        s(src), nCf(cfNom),
        trcf(NULL),
        mTrChart(masterTrChart)
{
    m_ui->setupUi(this);

    connect(&s,SIGNAL(updateData(int)),this,SLOT(updateDataRaw(int))); // при отриманні нових даних, засвітити їх на картинці
    //connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
    cb << m_ui->X_kon
        << m_ui->X_nds
        << m_ui->X_ndw
        << m_ui->X_nup
        << m_ui->X_nvs
        << m_ui->X_sgr_b
        << m_ui->X_sgr_z
        << m_ui->X_shib
        << m_ui->X_torm
        << m_ui->Y_nds
        << m_ui->Y_ndw
        << m_ui->Y_nup
        << m_ui->Y_pr
        << m_ui->Y_sgr
        << m_ui->Y_sir ;

    cb_name << "X_kon"
            << "X_nds"
            << "X_ndw"
            << "X_nup"
            << "X_nvs"
            << "X_sgr_b"
            << "X_sgr_z"
            << "X_shib"
            << "X_torm"
            << "Y_nds"
            << "Y_ndw"
            << "Y_nup"
            << "Y_pr"
            << "Y_sgr"
            << "Y_sir";

    State << tr("Тест")        //-1
          << tr("Зупинено")    // 0
          << tr("Пуск")        // 1
          << tr("Очікування")  // 2
          << tr("Завантаження") // 3
          << tr("Завантаження") // 4
          << tr("Розгон") //5
          << tr("Пром. лотка") // 6
          << tr("Фугування") // 7
          << tr("Пром. сироп.") // 8
          << tr("Пром. сироп.") // 9
          << tr("Пром. цукру") // 10
          << tr("Пром. цукру") // 11
          << tr("Пром. цукру") // 12
          << tr("Пром. цукру") // 13
          << tr("Пром. цукру") // 14
          << tr("Сушіння") // 15
          << tr("Гальмування") // 16
          << tr("Вивантаження") // 17
          << tr("Пром. сит") // 18
          << tr("Вібрація") // 19
          << tr("Синхронізація"); // 20

        cb_state
                << m_ui->cb_stop
                << m_ui->cb_wait
                << m_ui->cb_load
                << m_ui->cb_prl
                << m_ui->cb_fug
                << m_ui->cb_sir
                << m_ui->cb_pr
                << m_ui->cb_nt
                << m_ui->cb_vigr
                << m_ui->cb_reg;

    // елементи управління
    connect(m_ui->Gzag_zd,SIGNAL(valueChanged(double)),this,SLOT(sendValue(double)));
    connect(m_ui->Q_prl_zd,SIGNAL(valueChanged(double)),this,SLOT(sendValue(double)));
    connect(m_ui->Q_k,SIGNAL(valueChanged(double)),this,SLOT(sendValue(double)));
    connect(m_ui->Q_reg_zd,SIGNAL(valueChanged(double)),this,SLOT(sendValue(double)));

    connect(m_ui->Fott_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));
    connect(m_ui->T_fug_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));
    connect(m_ui->T_sir_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));
    connect(m_ui->Tnt_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));

    connect(m_ui->cf_1,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_2,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_3,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_4,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_5,SIGNAL(clicked()),this,SLOT(changeCf()));

    QVector<QRadioButton *> cf;
    cf << m_ui->cf_1
            << m_ui->cf_2
            << m_ui->cf_3
            << m_ui->cf_4
            << m_ui->cf_5;

    cf[nCf]->setChecked(true);

    changeCf();

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrChart()));
    t->start();

}

OneMnemo::~OneMnemo()
{
    //qDebug() << "OneMnemo: call destructor";
    delete trcf;
    delete m_ui;
}

void OneMnemo::updateDataRaw(int i) // слот обновляє дані на мнемосхемі
{
    if(i==nCf) // якщо моя центрифуга
    {
        int j=0;
        foreach(QCheckBox* v,cb)
        {
            v->setCheckState(s[i]->getValue16(cb_name[j])?Qt::Checked: Qt::Unchecked);
            ++j;
        }
        m_ui->Nc->setText(QString("%1").arg(s[i]->getValue32("Nc")));
        m_ui->Gzag->setText(QString("%1").arg(s[i]->getValueFloat("Gzag"),4,'f',0));
        m_ui->Q_all->setText(QString("%1").arg(s[i]->getValueFloat("Q_all"),4,'f',1));
        m_ui->Q_prl->setText(QString("%1").arg(s[i]->getValueFloat("Q_prl"),4,'f',1));
        m_ui->Q_pr_zd->setText(QString("%1").arg(s[i]->getValueFloat("Q_pr_zd"),4,'f',1));
        m_ui->Q_pr->setText(QString("%1").arg(s[i]->getValueFloat("Q_pr"),4,'f',1));
        m_ui->Q_reg->setText(QString("%1").arg(s[i]->getValueFloat("Q_reg"),4,'f',1));


        m_ui->Status->setText(State[s[i]->getValue16("Status")+1]);

        m_ui->Ob->setText(QString("%1").arg(s[i]->getValue16("Ob")));
        m_ui->I_m->setText(QString("%1").arg(s[i]->getValue16("I_m")));
        m_ui->T_prl->setText(QString("%1").arg(s[i]->getValue16("T_prl")));
        m_ui->Fott->setText(QString("%1").arg(s[i]->getValue16("Fott")));
        m_ui->T_fug->setText(QString("%1").arg(s[i]->getValue16("T_fug")));
        //m_ui->T_sir->setText(QString("%1").arg(s[i]->getValue16("T_sir")));
        m_ui->T_pr->setText(QString("%1").arg(s[i]->getValue16("T_pr")));
        m_ui->Imp->setText(QString("%1").arg(s[i]->getValue16("Imp")+1));
        m_ui->T_p_zd->setText(QString("%1").arg(s[i]->getValue16("T_p_zd")+1));
        m_ui->T_nt->setText(QString("%1").arg(s[i]->getValue16("T_nt")+1));
        m_ui->T_reg->setText(QString("%1").arg(s[i]->getValue16("T_reg")+1));
        m_ui->T_all->setText(QString("%1").arg(s[i]->getValue16("T_all")+1));

    }
}

void OneMnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{

}

void OneMnemo::updateTrChart() // поновлюємо дані на графічках
{
    QVector<double> v;
    trcf->addPoint(v);
}

void OneMnemo::changeCf()
{

    if(sender()) // якщо відправлено не самим класом при ініціалізації
        nCf=sender()->objectName().right(1).toInt()-1;
    //qDebug() << "Selected cf" << nCf;

    if(trcf)
    {
        delete trcf;
    }
    trcf=new TrendChart(m_ui->trcf,mTrChart[nCf]);

    if(nCf<s.size())
 {


    m_ui->Gzag_zd->blockSignals(true);
    m_ui->Gzag_zd->setValue(s[nCf]->getValueFloat("Gzag_zd"));
    m_ui->Gzag_zd->blockSignals(false);

    m_ui->Q_prl_zd->blockSignals(true);
    m_ui->Q_prl_zd->setValue(s[nCf]->getValueFloat("Q_prl_zd"));
    m_ui->Q_prl_zd->blockSignals(false);

    m_ui->Q_k->blockSignals(true);
    m_ui->Q_k->setValue(s[nCf]->getValueFloat("Q_k"));
    m_ui->Q_k->blockSignals(false);

    m_ui->Q_reg_zd->blockSignals(true);
    m_ui->Q_reg_zd->setValue(s[nCf]->getValueFloat("Q_reg_zd"));
    m_ui->Q_reg_zd->blockSignals(false);

    m_ui->Fott_zd->blockSignals(true);
    m_ui->Fott_zd->setValue(s[nCf]->getValue16("Fott_zd"));
    m_ui->Fott_zd->blockSignals(false);

    m_ui->T_fug_zd->blockSignals(true);
    m_ui->T_fug_zd->setValue(s[nCf]->getValue16("T_fug_zd"));
    m_ui->T_fug_zd->blockSignals(false);

    m_ui->T_sir_zd->blockSignals(true);
    m_ui->T_sir_zd->setValue(s[nCf]->getValue16("T_sir_zd"));
    m_ui->T_sir_zd->blockSignals(false);

    m_ui->Tnt_zd->blockSignals(true);
    m_ui->Tnt_zd->setValue(s[nCf]->getValue16("Tnt_zd"));
    m_ui->Tnt_zd->blockSignals(false);
  }
}

void OneMnemo::sendValue(int v)
{
    s[nCf]->sendValue(sender()->objectName(),v);
}

void OneMnemo::sendValue(double v)
{
    s[nCf]->sendValue(sender()->objectName(),v);
}
