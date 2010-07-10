
#include "mnemo.h"
#include "ui_mnemo.h"

#include <IoNetClient.h>

#include <QVBoxLayout>


Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);

    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateDataRaw()
{

}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{

}
