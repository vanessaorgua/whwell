#include <QtGui>

#include "ui_mainform.h"

#include <trend.h>

class IoNetClient;

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

private:
    
    IoNetClient &src; // вказівник на джерело даних

    TrendWindow *pTrw; // вказівник на вікно трендів
    struct trendinfo tp;
    Ui::Form *m_ui;
    QStack<QString> alertList;
};

