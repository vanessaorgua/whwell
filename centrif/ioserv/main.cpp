#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include "reportwr.h"

#include "iodev.h"
#include "RxModbus.h"
#include "logging.h"
#include "IoNetServer.h"
#include "alert.h"


//#include "display.h"
#include "form.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Rabitsa");
    QCoreApplication::setApplicationName("centrif");

    QSettings set;
    set.setValue("/db/db","centrif");


    QVector<RxModbus*> r;
    QVector<IoDev*> src; // джерела даних.

    for(int i=0;i<4;++i)
    {
        RxModbus *t= new RxModbus;
        t->setObjectName(QString("cf_%1").arg(i+1));
        t->loadList(":/text/list.txt");
        t->setHostName(QString("10.0.2.%1").arg(i+1));
        t->setPort(502);
        r << t;
        src << t;
    }

    {
        RxModbus *t= new RxModbus;
        t->setObjectName("cf_cmn");
        t->loadList(":/text/list2.txt");
        t->setHostName("10.0.1.0");
        t->setPort(502);
        r << t;
        src << t;
    }
    Logging l(src,1010); // писалка історіх
    QStringList tables;
    tables << "Cf_1" << "Cf_2" << "Cf_3" << "Cf_4" << "trend" ;
    l.setTables(tables);

    IoNetServer s(src); // мережевий інтерфейс

    ReportWiter rep(src);

    //Alert al(&r);
    //al.loadList(":/text/Linovitsa/filters/text/alert.txt");
    //QObject::connect(&r,SIGNAL(updateData()),&al,SLOT(checkAlert()));
    //QObject::connect(&al,SIGNAL(newAlert(QString)),&s,SLOT(sendAlert(QString)));
    for(int i=0;i<5;++i)
    {
        if(i<4) QObject::connect(r[i],SIGNAL(updateData()),&rep,SLOT(checkState()));
        QObject::connect(r[i],SIGNAL(Alert(QString)),&s,SLOT(sendAlert(QString)));
        r[i]->start();
    }
//    r[0]->start();

    qDebug() << "Started" ;
    return a.exec();
}

