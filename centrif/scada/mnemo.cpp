
#include "mnemo.h"
#include "ui_mnemo.h"
#include <trendchart.h>

#include <IoNetClient.h>

#include <QVBoxLayout>


Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);

//    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
//    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці
    connect(&src,SIGNAL(updateData(int)),this,SLOT(updateDataRaw(int)));

    // налаштування маленьких графіків
    QVector<QFrame*> trc;
    trc << m_ui->trcf_1
        << m_ui->trcf_2
        << m_ui->trcf_3
        << m_ui->trcf_4   ;
    QVector<QColor> clr;
    clr << Qt::green << Qt::red << Qt::yellow << Qt::cyan << Qt::black << Qt::black << Qt::black << Qt::black;

    for(int i=0;i<4;++i)
    {
        TrendChart *t=new TrendChart(trc[i],600,1);
        trcf<< t;
        t->setColor(clr);
        QVBoxLayout *vbl=new QVBoxLayout(trc[i]);
        vbl->addWidget(t);
        trc[i]->setLayout(vbl);
    }

    // заповнити масиви вказівників
    {
    QVector<QCheckBox*> tmp;
    tmp << m_ui->X_kon_1
        << m_ui->X_nds_1
        << m_ui->X_ndw_1
        << m_ui->X_nup_1
        << m_ui->X_nvs_1
        << m_ui->X_sgr_b_1
        << m_ui->X_sgr_z_1
        << m_ui->X_shib_1
        << m_ui->X_torm_1
        << m_ui->Y_nds_1
        << m_ui->Y_ndw_1
        << m_ui->Y_nup_1
        << m_ui->Y_pr_1
        << m_ui->Y_sgr_1
        << m_ui->Y_sir_1 ;
    cb << tmp;
    tmp.clear();
    tmp << m_ui->X_kon_2
        << m_ui->X_nds_2
        << m_ui->X_ndw_2
        << m_ui->X_nup_2
        << m_ui->X_nvs_2
        << m_ui->X_sgr_b_2
        << m_ui->X_sgr_z_2
        << m_ui->X_shib_2
        << m_ui->X_torm_2
        << m_ui->Y_nds_2
        << m_ui->Y_ndw_2
        << m_ui->Y_nup_2
        << m_ui->Y_pr_2
        << m_ui->Y_sgr_2
        << m_ui->Y_sir_2 ;
    cb << tmp;
    tmp.clear();
    tmp << m_ui->X_kon_3
        << m_ui->X_nds_3
        << m_ui->X_ndw_3
        << m_ui->X_nup_3
        << m_ui->X_nvs_3
        << m_ui->X_sgr_b_3
        << m_ui->X_sgr_z_3
        << m_ui->X_shib_3
        << m_ui->X_torm_3
        << m_ui->Y_nds_3
        << m_ui->Y_ndw_3
        << m_ui->Y_nup_3
        << m_ui->Y_pr_3
        << m_ui->Y_sgr_3
        << m_ui->Y_sir_3 ;
    cb << tmp;
    tmp.clear();
    tmp << m_ui->X_kon_4
        << m_ui->X_nds_4
        << m_ui->X_ndw_4
        << m_ui->X_nup_4
        << m_ui->X_nvs_4
        << m_ui->X_sgr_b_4
        << m_ui->X_sgr_z_4
        << m_ui->X_shib_4
        << m_ui->X_torm_4
        << m_ui->Y_nds_4
        << m_ui->Y_ndw_4
        << m_ui->Y_nup_4
        << m_ui->Y_pr_4
        << m_ui->Y_sgr_4
        << m_ui->Y_sir_4 ;
    cb << tmp;
}
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
    {
        QVector<QLineEdit*> tmp;
        tmp << m_ui->Ob_1
            << m_ui->G_zag_1
            << m_ui->Q_all_1
            << m_ui->I_m_1
            << m_ui->Status_1;
        le << tmp;

        tmp.clear();
        tmp << m_ui->Ob_2
            << m_ui->G_zag_2
            << m_ui->Q_all_2
            << m_ui->I_m_2
            << m_ui->Status_2;
        le << tmp;

        tmp.clear();
        tmp << m_ui->Ob_3
            << m_ui->G_zag_3
            << m_ui->Q_all_3
            << m_ui->I_m_3
            << m_ui->Status_3;
        le << tmp;

        tmp.clear();
        tmp << m_ui->Ob_4
            << m_ui->G_zag_4
            << m_ui->Q_all_4
            << m_ui->I_m_4
            << m_ui->Status_4;
        le << tmp;

    }


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



    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrChart()));
    t->start();

    connect(m_ui->bn_Cf_1,SIGNAL(clicked()),p,SLOT(showOneMnemo()));
    connect(m_ui->bn_Cf_2,SIGNAL(clicked()),p,SLOT(showOneMnemo()));
    connect(m_ui->bn_Cf_3,SIGNAL(clicked()),p,SLOT(showOneMnemo()));
    connect(m_ui->bn_Cf_4,SIGNAL(clicked()),p,SLOT(showOneMnemo()));

    // це відноситься до загальних параметрів
    le_cm
            << m_ui->le_V_01
            << m_ui->le_V_02
            << m_ui->le_V_03
            << m_ui->le_V_04
            << m_ui->le_V_05
            << m_ui->le_V_06
            << m_ui->le_V_07
            << m_ui->le_V_08;
    pb_cm
            << m_ui->pb_V_05
            << m_ui->pb_V_06
            << m_ui->pb_V_07
            << m_ui->pb_V_08;
    cb_cm
            << m_ui->cb_AM_01
            << m_ui->cb_X_01
            << m_ui->cb_X_04
            << m_ui->cb_X_05
            << m_ui->cb_X_06
            << m_ui->cb_X_07
            << m_ui->cb_Y_01;
}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateDataRaw(int i)
{
/*    QString r;
    foreach(QString str,s[i]->getTags().keys())
    {
        r+=QString("%1=%2; ").arg(str).arg(s[i]->getValue16(str));

    }
    qDebug() << r;*/

    if(i<4) // якщо сигнал від контролера центрифуг
    {
        //qDebug() << "elements" << cb[i].size() << cb_name.size();
        int j=0;
        foreach(QCheckBox* v,cb[i])
        {
            v->setCheckState(s[i]->getValue16(cb_name[j])?Qt::Checked: Qt::Unchecked);
            //qDebug() << i << j << cb_name[j] << s[i]->getValue16(cb_name[j]);
            ++j;
        }

        le[i][1]->setText(QString("%1").arg(s[i]->getValueFloat("Gzag"),4,'f',0));
        le[i][2]->setText(QString("%1").arg(s[i]->getValueFloat("Q_all"),3,'f',0));

        le[i][0]->setText(QString("%1").arg(s[i]->getValue16("Ob")));
        le[i][3]->setText(QString("%1").arg(s[i]->getValueFloat("I_m"),4,'f',0));

        if(s[i]->getValue16("Status")>-2 && s[i]->getValue16("Status")< 18)
        {
            le[i][4]->setText(State[s[i]->getValue16("Status")+1]);
        }

    }
    else if(i==4)
    {
    // це загальні парамерти
        foreach(QProgressBar*p,pb_cm)
        {
            p->setValue(s[4]->getValueFloat(p->objectName().right(p->objectName().size()-3)));
        }

        foreach(QLineEdit*p,le_cm)
        {
            p->setText(QString("%1").arg(s[4]->getValueScaled(p->objectName().right(p->objectName().size()-3)),3,'f',0));
        }

        foreach(QCheckBox*p,cb_cm)
        {
            p->setChecked(s[4]->getValue16(p->objectName().right(p->objectName().size()-3)));
        }

        m_ui->bn_X_03->setIcon(QIcon(QPixmap(s[4]->getValue16("X_03")
            ?":/pict/lib/valve_green_20x32.png":":/pict/lib/valve_off_20x32.png")));
    }
}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{
    // це загальні параметри
    foreach(QLineEdit*p,le_cm)
    {
        p->setText(QString("%1").arg(s[4]->getValueScaled(p->objectName().right(p->objectName().size()-3)),3,'f',0));
    }
}

void Mnemo::updateTrChart() // поновлюємо дані на графічках
{
    int n=s.size();
    // qDebug() << "updateTrChart() " << n;
    if(n>4) n=4;
    for(int i=0;i<n;++i)
    {
        double sgr;
        if( s[i]->getValue16("X_sgr_b") && !s[i]->getValue16("X_sgr_z")) //Формування змінної Sgr_t для відображення графіку положення сегрегатора на панелі*)
            sgr=400.0;
        else if( s[i]->getValue16("X_sgr_z") && !s[i]->getValue16("X_sgr_b") ) // X_sgr_z AND NOT X_sgr_b THEN
            sgr=3600.0;
        else
            sgr=2000.0;
        //qDebug() << "tags size" << s[i]->getTags().keys().size();

        QVector<double> v;
        v<< (double)s[i]->getValue16("Ob") *3.33 << s[i]->getValueFloat("Q_all")*40.0 << s[i]->getValue16("Fott")*40 << sgr ;
        v << 0.0 << 0.0;
        trcf[i]->addPoint(v);

    }

}




