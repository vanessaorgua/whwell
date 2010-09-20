#ifndef __ONE_MNEMO_H__
#define __ONE_MNEMO_H__

#include <QLabel>
#include <QVector>

class TrendChart;
class IoNetClient;
class QCheckBox;
class QLineEdit;

namespace Ui {
    class one_mnemo;
}

class OneMnemo: public QLabel
{
    Q_OBJECT
public:
    OneMnemo(IoNetClient &src, QVector<TrendChart*> &masterTrChart,  int cfNom=1, QWidget *p=NULL);
    ~OneMnemo();

    inline int currentCfNom(){ return nCf;}

public slots:
    void updateDataRaw(int); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void updateTrChart(); // поновлюємо дані на графічках
    void changeCf();
    void sendValue(int);
    void sendValue(double);

private:
    Ui::one_mnemo *m_ui;
    IoNetClient &s;
    int nCf;
    // масиви вказівників на елементи управління
    TrendChart* trcf;

    QVector<QCheckBox*>  cb; // тут будуть дискретні індикатори
    QVector<QCheckBox*> cb_state; // це лампочки на панельці

    QStringList cb_name; // імена тегів для вивантаження

    QVector<QVector<QLineEdit*> > le; // тут будуть рядки

    QStringList State; // список станів
    QVector<TrendChart*> &mTrChart;
};

#endif
