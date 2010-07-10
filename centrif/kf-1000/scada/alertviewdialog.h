#ifndef ALERTVIEWDIALOG_H
#define ALERTVIEWDIALOG_H

#include <QtGui/QDialog>
#include <QStack>

namespace Ui {
    class AlertViewDialog;
}


class AlertViewDialog : public QDialog {
    Q_OBJECT
public:
    AlertViewDialog(QStack<QString> v, QWidget *parent = 0);
    ~AlertViewDialog();
public slots:
    void updateAlert(QString);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AlertViewDialog *m_ui;
};

#endif // ALERTVIEWDIALOG_H
