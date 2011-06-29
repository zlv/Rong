#include <QPainter>
#include "platform.h"
#include "circleofdeath.h"

Platform::Platform(int i, QGraphicsItem *parent) :
    QGraphicsItem(parent), _index(i){}

QRectF Platform::boundingRect() const
{
    return QRectF();
}

void Platform::paint(QPainter *, const QStyleOptionGraphicsItem *,
                     QWidget *)
{
    //p->drawRect(0,0,50,50);
}
