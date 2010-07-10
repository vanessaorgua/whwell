#include "history.h"
#include <iodev.h>
#include "ui_history.h"

#include <QDebug>



RHistorySelect::RHistorySelect(IoDev &src,struct trendinfo *tp,QWidget *p /*=NULL*/) :
        QDialog(p),
        s(src),
        TrendParam(tp),
        m_ui(new Ui::History)
{
    m_ui->setupUi(this);

}

RHistorySelect::~RHistorySelect()
{
    delete m_ui;
}

void RHistorySelect::slotAccept()
{
    nameTrend=sender()->objectName();
    // тут треба завантажити дані в структуру 

    QFile f(QString(":/text/%1").arg(nameTrend));
    QString t;
    QStringList sl;
    QSettings set;

    TrendParam->host=set.value("/db/hostname","localhost").toString();
    TrendParam->db=set.value("/db/dbname","lynovycya").toString();
    TrendParam->user=set.value("/db/username","scada").toString();
    TrendParam->passwd=set.value("/db/passwd","").toString();
    
    
    if(f.open(QIODevice::ReadOnly))
    {
        int i;
        sl.clear();

        for(i=0;!f.atEnd() && i<8;++i) // обмежети зчитування із файла кінцем файла або не більше як 8 рядків
	{
                TrendParam->fields[i]=t=QString::fromUtf8(f.readLine()).trimmed(); // прочитати назву поля
                if(s.getTags().contains(t)) // якщо задане поле знайдено
		{
                    sl<< /*s.getText()[t].size() > 0 ? */s.getText()[t] /*: t */; // завантажити назву поля, якщо не знайдено - назву тега


                    TrendParam->fScale[i][0]=s.scaleZero(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму
                    TrendParam->fScale[i][1]=s.scaleFull(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму

                     if(s.fieldType(t)==1) // якщо дискретний сигнал
                    {
			    // змінити тип поля
                            TrendParam->fields[i]=QString("((%1!=0)*454+%2)").arg(t).arg(i*499);
                            // дискретні шкали
			    TrendParam->fScale[i][0]=0.0-1.1*(double)i;
			    TrendParam->fScale[i][1]=8.8-1.1*(double)i;
			}
		}
		else
		{--i;} // можливо і поганий варіант яле якщо такого поля не знайдено тоді змінити лічильник циклів
	}

	TrendParam->numPlot=i; // завантажити кількість графіків
	TrendParam->table="trend";
	TrendParam->trend=sender()->objectName(); // Завантажити ім’я тренда
	
	TrendParam->trendHead=qobject_cast<QPushButton*>(sender())->text(); // заголовок тренда - те, що написано на кнопці
	TrendParam->fieldHead = sl;

	//qDebug() << "1 TrendParam->numPlot=" << TrendParam->numPlot;
	//qDebug() << "2 TrendParam->table  =" << TrendParam->table;
	//qDebug() << "3 TrendParam->trend  =" << TrendParam->trend; // Завантажити ім’я тренда
	//for(i=0;i<TrendParam->numPlot;++i)
	//    qDebug() << "4 TrendParam->fields[" << i << "]=" << TrendParam->fields[i];
	
	//for(i=0;i<TrendParam->numPlot;++i)
	//    qDebug() << "5 TrendParam->fScale["<<i<<"] =" << TrendParam->fScale[i][0] << TrendParam->fScale[i][1];
	    
	//qDebug() << "6 TrendParam->trendHead=" << TrendParam->trendHead; // заголовок тренда - те, що написано на кнопці
	//qDebug() << "7 TrendParam->fieldHead =" << TrendParam->fieldHead;

	f.close();
	accept(); // для завершення роботи
    }
    else
	reject(); // якщо не вдалося відкрити відповідний файл


}

void RHistorySelect::changeEvent(QEvent *e)
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
