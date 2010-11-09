#include "history.h"
#include"IoNetClient.h"
#include "ui_history.h"

#include <QDebug>



RHistorySelect::RHistorySelect(IoNetClient &src,struct trendinfo *tp,QWidget *p /*=NULL*/) :
        QDialog(p),
        s(src),
        TrendParam(tp),
        m_ui(new Ui::History)
{
    m_ui->setupUi(this);

    connect(m_ui->Cf_10cf1,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_20cf2,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_30cf3,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_40cf4,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->trend0trend5,SIGNAL(clicked()),this,SLOT(slotAccept()));

    connect(m_ui->Cf_10drv1,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_20drv2,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_30drv3,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_40drv4,SIGNAL(clicked()),this,SLOT(slotAccept()));

    connect(m_ui->Cf_10upload1,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_20upload2,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_30upload3,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_40upload4,SIGNAL(clicked()),this,SLOT(slotAccept()));

    connect(m_ui->Cf_10other1,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_20other2,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_30other3,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_ui->Cf_40other4,SIGNAL(clicked()),this,SLOT(slotAccept()));

}

RHistorySelect::~RHistorySelect()
{
    delete m_ui;
}

void RHistorySelect::slotAccept()
{

    QStringList fl=sender()->objectName().split('0'); // символ 0 є розподілювачам імені таблиці та імені файлу із списком полів
    nameTrend=fl[1].left(fl[1].size()-1);
    int nIo=fl[1].right(1).toInt()-1;
    // тут треба завантажити дані в структуру

    QFile f(QString(":/text/%1").arg(nameTrend));
    QString t;
    QStringList sl,sl2;
    QSettings set;

    TrendParam->host=set.value("/db/hostname","localhost").toString();
    TrendParam->db=set.value("/db/dbname","test").toString();
    TrendParam->user=set.value("/db/username","scada").toString();
    TrendParam->passwd=set.value("/db/passwd","").toString();
    
    
    if(f.open(QIODevice::ReadOnly))
    {
        int i;
        sl.clear();

        for(i=0;!f.atEnd() && i<8;++i) // обмежети зчитування із файла кінцем файла або не більше як 8 рядків
	{
                t=    QString::fromUtf8(f.readLine()).trimmed();
                //qDebug() << t;
                sl2=t.split('\t');

                TrendParam->fields[i]=t=sl2[0]; // прочитати назву поля
                if(s[nIo]->getTags().contains(t)) // якщо задане поле знайдено
		{
                    QString hI=s[nIo]->getText()[t];
                    if(hI=="")
                        hI=t;
                    sl<< /*s.getText()[t].size() > 0 ? */hI /*: t */; // завантажити назву поля, якщо не знайдено - назву тега


                    TrendParam->fScale[i][0]=s[nIo]->scaleZero(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму
                    TrendParam->fScale[i][1]=s[nIo]->scaleFull(t); // спробувати розпізнати тип поля та/чи значення шкали мінімуму

                     if(s[nIo]->fieldType(t)==1) // якщо дискретний сигнал
                    {
			    // змінити тип поля
                            TrendParam->fields[i]=QString("((%1!=0)*400+%2)").arg(t).arg(i*500);
                            // дискретні шкали
                            TrendParam->fScale[i][0]=0-1*(double)i;
                            TrendParam->fScale[i][1]=8-1*(double)i;
			}
		}
		else
                {
                    qDebug() << sl2;
                    if(sl2.size()>3) // якщо дані є  в файлі конфігурації тоді зчитати їх звідти
                    {
                        TrendParam->fScale[i][0]=sl2[1].toDouble(); // прочитати що там є
                        TrendParam->fScale[i][1]=sl2[2].toDouble(); //
                        sl << sl2[3];
                    }
                    else
                        --i;
                } // можливо і поганий варіант яле якщо такого поля не знайдено тоді змінити лічильник циклів
	}

        //qDebug() << "----------";
	TrendParam->numPlot=i; // завантажити кількість графіків
        TrendParam->table=fl[0];
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
