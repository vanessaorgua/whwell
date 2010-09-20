#ifndef __MNEMO_H__
#define __MNEMO_H__

#include <QLabel>
#include <QVector>

class TrendChart;
class IoNetClient;
class QCheckBox;
class QLineEdit;
class QProgressBar;

namespace Ui {
    class mnemo;
}

class Mnemo: public QLabel
{
    Q_OBJECT
public:
    Mnemo(IoNetClient &src, QWidget *p=NULL);
    ~Mnemo();
    QVector<TrendChart*> &getTrChart() {return trcf;}

public slots:
    void updateDataRaw(int); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrChart(); // поновлюємо дані на графічках

    void slotShibCtrl();

private:
    Ui::mnemo *m_ui;
    IoNetClient &s;
    // масиви вказівників на елементи управління
    QVector<TrendChart*> trcf;

    QVector<QVector<QCheckBox*> > cb; // тут будуть дискретні індикатори
    QStringList cb_name; // імена тегів для вивантаження

    QVector<QVector<QLineEdit*> > le; // тут будуть рядки

    QStringList State; // список станів

    // це відноситься до загальних параметрів
    QVector<QLineEdit*> le_cm;
    QVector<QProgressBar*> pb_cm;
    QVector<QCheckBox*> cb_cm;

};

#endif
