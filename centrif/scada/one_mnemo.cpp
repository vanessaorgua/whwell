
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
          << tr("Пром. сит") // 2
          << tr("Очікування")  // 3
          << tr("Завантаження") // 4
          << tr("Фугування") // 5
          << tr("Пром. сироп.") // 6
          << tr("Пром. сироп.") // 7
          << tr("Пром. цукру") // 8
          << tr("Пром. цукру") // 9
          << tr("Пром. цукру") // 10
          << tr("Пром. цукру") // 11
          << tr("Пром. цукру") // 12
          << tr("Сушіння") // 13
          << tr("Гальмування") // 14
          << tr("Вивантаження") // 15
          << tr("Синхронізація") // 16
          << tr("Вібрація"); // 17


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
    connect(m_ui->T_zag_zd,SIGNAL(valueChanged(int)),this,SLOT(sendValue(int)));

    connect(m_ui->cf_1,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_2,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_3,SIGNAL(clicked()),this,SLOT(changeCf()));
    connect(m_ui->cf_4,SIGNAL(clicked()),this,SLOT(changeCf()));

    QVector<QRadioButton *> cf;
    cf << m_ui->cf_1
            << m_ui->cf_2
            << m_ui->cf_3
            << m_ui->cf_4;

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


        m_ui->Ob->setText(QString("%1").arg(s[i]->getValue16("Ob")));
        m_ui->I_m->setText(QString("%1").arg(s[i]->getValueFloat("I_m"),4,'f',0));
        m_ui->T_prl->setText(QString("%1").arg(s[i]->getValue16("T_prl")));
        m_ui->Fott->setText(QString("%1").arg(s[i]->getValue16("Fott")));
        m_ui->T_fug->setText(QString("%1").arg(s[i]->getValue16("T_fug")));
        //m_ui->T_sir->setText(QString("%1").arg(s[i]->getValue16("T_sir")));
        m_ui->T_pr->setText(QString("%1").arg(s[i]->getValue16("T_pr")));
        m_ui->Imp->setText(QString("%1").arg(s[i]->getValue16("Imp")));
        m_ui->T_p_zd->setText(QString("%1").arg(s[i]->getValue16("T_p_zd")));
        m_ui->T_nt->setText(QString("%1").arg(s[i]->getValue16("T_nt")));
        m_ui->T_reg->setText(QString("%1").arg(s[i]->getValue16("T_reg")));
        m_ui->T_all->setText(QString("%1").arg(s[i]->getValue16("T_all")));

        m_ui->T_zag->setText(QString("%1").arg(s[i]->getValue16("T_zag")/1000));

        int Status = s[i]->getValue16("Status");
        m_ui->Status->setText(State[Status+1]);

        m_ui->cb_prl->setChecked(s[i]->getValue16("Y_prl"));

        if(Status!=lastStatus)
        {
            foreach(QCheckBox *cb,cb_state)
            {
                cb->setChecked(false);
                switch(Status)
                {
                    case 0:
                        m_ui->cb_stop->setChecked(true);
                        break;
                case 2:
                    m_ui->cb_reg->setChecked(true);
                    break;
                case 3:
                    m_ui->cb_wait->setChecked(true);
                    break;
                case 4:
                    m_ui->cb_load->setChecked(true);
                    break;
                case 5:
                    m_ui->cb_fug->setChecked(true);
                    break;
                case 6:
                case 7:
                    m_ui->cb_sir->setChecked(true);
                    break;
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                    m_ui->cb_pr->setChecked(true);
                    break;
                case 13:
                case 14:
                    m_ui->cb_nt->setChecked(true);
                    break;
                case 15:
                    m_ui->cb_vigr->setChecked(true);
                    break;
                default:
                    break;

                }
            }
        }
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

    m_ui->T_zag_zd->blockSignals(true);
    m_ui->T_zag_zd->setValue(s[nCf]->getValue16("T_zag_zd"));
    m_ui->T_zag_zd->blockSignals(false);

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
