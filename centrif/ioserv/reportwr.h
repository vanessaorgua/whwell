#ifndef REPORTWITER_H
#define REPORTWITER_H

#include <QObject>
#include <QVector>


class IoDev;

class ReportWiter : public QObject
{
    Q_OBJECT
public:
    ReportWiter(QVector<IoDev*> &source);

public slots:
    void checkState();

private:
    QVector<IoDev*> &src;

    QVector<int> lastState;
    QVector<QVector<int> > data;

};

#endif // REPORTWITER_H
