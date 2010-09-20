#include "cfparam.h"
#include "ui_cfparam.h"
#include <iodev.h>

CfParam::CfParam(IoDev &source, QWidget *parent) :
    QDialog(parent),
    src(source),
    m_ui(new Ui::CfParam)
{
    m_ui->setupUi(this);

    dsb << m_ui->Kv_1
        << m_ui->Kv_2
        << m_ui->K_imp
        << m_ui->Kf_prl
        << m_ui->Kf_pr
        << m_ui->Q_reg_zd
        << m_ui->Q_prl_zd
        << m_ui->Q_man_zd
        << m_ui->Max_gzag
        << m_ui->Fott_min
        << m_ui->Fott_max
        << m_ui->Ob_max
        << m_ui->I_m_max;

    sb  << m_ui->T_zag_zd
        << m_ui->T_fug_zd
        << m_ui->T_sir_zd
        << m_ui->T_p_zd

        << m_ui->Ts_z_zd
        << m_ui->Ts_b_zd
        << m_ui->Tn_v
        << m_ui->Tn_n
        << m_ui->T_vigr_zd
        << m_ui->Fott_zd
        << m_ui->Ob_zag_zd
        << m_ui->Ob_prl_zd
        << m_ui->Ob_fug_zd
        << m_ui->Ob_vigr_zd
        << m_ui->Ob_del_zd

        << m_ui->Kf_i_m
        << m_ui->Kf_ob
        << m_ui->Kf_fott;

    dsb_tag <<"Kv_1"
        <<"Kv_2"
        <<"K_imp"
        <<"Kf_prl"
        <<"Kf_pr"
        <<"Q_reg_zd"
        <<"Q_prl_zd"
        <<"Q_man_zd"
        <<"Max_gzag"
        <<"Fott_min"
        <<"Fott_max"
        <<"Ob_max"
        <<"I_m_max";
    sb_tag <<"T_zag_zd"
        <<"T_fug_zd"
        <<"T_sir_zd"
        <<"T_p_zd"

        <<"Ts_z_zd"
        <<"Ts_b_zd"
        <<"Tn_v"
        <<"Tn_n"
        <<"T_vigr_zd"
        <<"Fott_zd"
        <<"Ob_zag_zd"
        <<"Ob_prl_zd"
        <<"Ob_fug_zd"
        <<"Ob_vigr_zd"
        <<"Ob_del_zd"
        <<"Kf_i_m"
        <<"Kf_ob"
        <<"Kf_fott";

    int i=0;
    foreach(QString str,dsb_tag)
    {
        dsb[i++]->setValue(src.getValueFloat(str));
    }
    i=0;
    foreach(QString str,sb_tag)
    {
        sb[i++]->setValue(src.getValue16(str));
    }
    m_ui->Imp->setCurrentIndex(src.getValue16("Imp"));
    m_ui->Rej_sir->setChecked(src.getValue16("Rej_sir"));
    m_ui->Rej_fug->setCurrentIndex(src.getValue16("Rej_fug")?1:0);

    connect(m_ui->buttonBox,SIGNAL(accepted()),this,SLOT(slotAccept()));

}

CfParam::~CfParam()
{
    delete m_ui;
}

void CfParam::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CfParam::slotAccept()
{
    int i=0;
    foreach(QString str,dsb_tag)
        src.sendValue(str,dsb[i++]->value());

    i=0;
    foreach(QString str,sb_tag)
        src.sendValue(str,qint16(sb[i++]->value()));

    src.sendValue("Imp",qint16(m_ui->Imp->currentIndex()));
    src.sendValue("Rej_sir",qint16(m_ui->Rej_sir->checkState()?-1:0));
    src.sendValue("Rej_fug",qint16(m_ui->Rej_fug->currentIndex()?-1:0));
    accept();
}


