#include "reportwr.h"
#include "iodev.h"
#include <QtSql>

ReportWiter::ReportWiter(QVector<IoDev*> &source):
        src(source)

{
    // ініціалізувати масив станів
    lastState.fill(0,5);

    // ініціалізувати масив даних
    QVector<int> v(11);
    data.fill(v,5) ;

}


void ReportWiter::checkState()
{
    int i=sender()->objectName().right(1).toInt(); // визначити хто відправив сигнал
    qint16 s=src[i]->getValue16("Status");
    if(lastState[i]==18 && s!=18) //якщо була регенерація а стала не регенерація - записати звіт
    {
        QSqlDatabase dbs=QSqlDatabase::database("logging",true); //знайти з’єдання logging
        if(dbs.isOpen()) // якщо база відкрита
        {
            QDateTime tm=QDateTime::currentDateTime();
            QSqlQuery query(dbs);
            // сформувати і виконати запит, перевірити результат
            if(! query.exec(QString("INSERT INTO report(cft,nCf,Nc,T_all,Gzag,Q_prl,T_prl,T_fug,Q_pr,T_pr,T_nt,Q_reg,T_reg, Q_man) "
                               "VALUES (\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\',\'%9\',\'%10\',\'%11\',\'%12\',\'%13\',\'%14\')")
                       .arg(tm.toString("yyyy/MM/dd hh:mm:ss"))
                       .arg(i+1)
                       .arg(src[i]->getValue32("Nc"))
                       .arg(data[i][0])
                       .arg(data[i][1])
                       .arg(data[i][2])
                       .arg(data[i][3])
                       .arg(data[i][4])
                       .arg(data[i][5])
                       .arg(data[i][6])
                       .arg(data[i][7])
                       .arg(data[i][8])
                       .arg(data[i][9])
                       .arg(data[i][10])            )
            )
            { // якщо помилка
                qDebug() << query.lastError().databaseText(); //на екран
                qDebug() << query.lastQuery();
            }
            dbs.close();
        }
    }

    if(s==18) // якщо стадія регенераці зберегти дані, бо після її закінчення контролер їх обнулить.
    {
        data[i][0]=src[i]->getValue16("T_all");
        data[i][1]=src[i]->getValueFloat("Gzag");
        data[i][2]=src[i]->getValueFloat("Q_prl");
        data[i][3]=src[i]->getValue16("T_prl");
        data[i][4]=src[i]->getValue16("T_fug");
        data[i][5]=src[i]->getValueFloat("Q_pr");
        data[i][6]=src[i]->getValue16("T_pr");
        data[i][7]=src[i]->getValue16("T_nt");
        data[i][8]=src[i]->getValueFloat("Q_reg");
        data[i][9]=src[i]->getValue16("T_reg");
        data[i][10]=src[i]->getValueFloat("Q_man");
    }
    lastState[i]=s; // зберегти стан для наступного виклику

}


