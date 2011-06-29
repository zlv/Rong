#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsItem>

class Platform : public QGraphicsItem
{
    int _index;
public:
    Platform(int, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *,QWidget*);
};

#endif // PLATFORM_H
