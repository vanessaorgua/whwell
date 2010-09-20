#ifndef CFPARAM_H
#define CFPARAM_H

#include <QtGui/QDialog>

class IoDev;
class QSpinBox;
class QDoubleSpinBox;

namespace Ui {
    class CfParam;
}

class CfParam : public QDialog {
    Q_OBJECT
public:
    CfParam(IoDev &source, QWidget *parent = 0);
    ~CfParam();
public slots:
      void slotAccept();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CfParam *m_ui;
    IoDev &src;

    QVector<QSpinBox*> sb;
    QVector<QDoubleSpinBox*> dsb;
    QStringList sb_tag,dsb_tag;


};

#endif // CFPARAM_H
