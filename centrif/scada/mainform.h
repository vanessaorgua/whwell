#include <QtGui>

#include "ui_mainform.h"


#include <trend.h>

class IoNetClient;
class OneMnemo;
class Mnemo;
class Report;

class mMainForm: public QWidget // ,public Ui::Form
{
    Q_OBJECT
public:
        mMainForm(IoNetClient &source,QWidget *p=NULL);
	~mMainForm();
	
public slots:
    void timeOut();
    void about();
    void trRun();
    void showMe();            
    void setupParm();
    
    void slotExit();
    void slotAlert(QString); // це буде викликатися коли сервер пришле алерт
    void showAlert();

    void showOneMnemo(); // ці дві функцію слугують для виводу мнемосхеми одиночної центрифуги
    void showReport();
    void showMainMnemo();

private:
    
    IoNetClient &src; // вказівник на джерело даних

    TrendWindow *pTrw; // вказівник на вікно трендів
    struct trendinfo tp;
    Ui::Form *m_ui;
    QStack<QString> alertList;

    // це мнемосхеми
    Mnemo *mMnemo;
    OneMnemo *oM;
    Report *rep;
    int currentIndex; // поточний ідекс 0 - відображення центрифуг 1- відображення звітів

};

