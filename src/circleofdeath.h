/*=========================================================================
==                           circleofdeath.h                             ==
==   CircleOfDeath -- ужаснейший круг смерти, внутри него ездеют 2 ваго- ==
== нетки, способные отталкивать мячи.                                    ==
==   Описание:                                                           ==
==   Круг, разделённый на две разные части, по каждой из которых может   ==
== ездить вагонетка, гоняющаяся за мячом. От сторон круга мячик может ли-==
== бо оттолкнуться, либо вызвать завершение игры. /                      ==
==                                                                       ==
==   CircleOfDeath -- class with 2 platforms, one circle, 2 circle limi- ==
== ters.                                                                 ==
=========================================================================*/
/*Авторы/Authors: zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь
             pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь*/
#ifndef CIRCLEOFDEATH_H
#define CIRCLEOFDEATH_H
#include <QGraphicsItem> //базовый класс, графический элемент qt
#include "platform.h" //одна из двух играющих вагонеток

class CircleOfDeath : public QGraphicsItem
{
    Platform* platform_[2]; //вагонетки
    double limiter_[2]; //углы ограничителей / limiters for circle of death
    static const double limitEmptySpace; //ширина ограничителей
    double radius_; //радиус круга смерти / for the circle of death

public:
    CircleOfDeath(QGraphicsItem *parent = 0);
    //унаследованные функции
    QRectF boundingRect() const;
    void paint(QPainter*,const QStyleOptionGraphicsItem *, QWidget*);
    QPainterPath shape() const;
    void addItems(); //добавить вагонетки / add platforms
};

#endif // CIRCLEOFDEATH_H
