#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <QtGui>

#include <trend.h>


class IoNetClient;

namespace Ui {
    class History;
}

class RHistorySelect: public QDialog
{
Q_OBJECT
public:
    RHistorySelect(IoNetClient &src,struct trendinfo *tp,QWidget *p=NULL);
    ~RHistorySelect() ;
    QString& getNameTrend() {return nameTrend;}

    //struct trendinfo* getTrendParam() { return &TrendParam ;}
private slots:
    void slotAccept();

protected:
    void changeEvent(QEvent *e);

private:
    QString nameTrend;
    struct trendinfo *TrendParam;
    IoNetClient &s;

    Ui::History *m_ui;

};

#endif

