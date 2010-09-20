#ifndef SPARAMDIALOG_H
#define SPARAMDIALOG_H

#include <QtGui/QDialog>

class IoNetClient;

namespace Ui {
    class SParamDialog;
}

class SParamDialog : public QDialog {
    Q_OBJECT
public:
    SParamDialog(IoNetClient &src,QWidget *parent = 0);
    ~SParamDialog();

private slots:
    void myAccept();
    void selectTeg(int v);

    void slotSet(QString);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::SParamDialog *m_ui;
    IoNetClient &s;
    int ch;

    QStringList tag_name; // список тегів, які треба налаштовувати
};

#endif // SPARAMDIALOG_H
