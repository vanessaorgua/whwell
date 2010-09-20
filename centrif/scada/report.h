#ifndef REPORT_H
#define REPORT_H

#include <QtGui/QWidget>

class QSqlQueryModel;

namespace Ui {
    class Report;
}

class Report : public QWidget {
    Q_OBJECT
public:
    Report(QWidget *parent = 0);
    ~Report();
public slots:
       void slotChangeCfN(int);
       void slotCallCalendar();
       void slotChangeZm(int);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Report *m_ui;
    QSqlQueryModel *mRepModel;

};

#endif // REPORT_H
