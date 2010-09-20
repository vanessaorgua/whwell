#include <QHash>
#include "display.h"
#include <iodev.h>


display::display(IoDev &source): src(source)
{
}

void display::slotDisplay()
{
    QString s;
    qDebug() << "-------------------------";
    foreach(s,src.getTags().keys())
    {
        qDebug() << s << src.getValue16(s);
    }

}


