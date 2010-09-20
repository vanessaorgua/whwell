#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>

class IoDev;

class display : public QObject
{
    Q_OBJECT
public:
    display(IoDev &source);
public slots:
    void slotDisplay();
private:
    IoDev &src;
};

#endif // DISPLAY_H
