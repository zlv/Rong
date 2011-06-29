/*=========================================================================
==                              ball.h                                   ==
==   Ball -- мячик.                                                      ==
==   Описание:                                                           ==
==   Мячиков может быть несколько, но вначале он только один, летает се- ==
== бе, летает, попадает на вагонетку, та его отталкивает. Ещё мячик дол- ==
== жен учитывать круг смерти, от которого он может оттолкнуться. Шарик   ==
== также может отталкиваться от других мячиков, для этого ему необходима ==
== ссылка на поле, где он находится (Field*).                            ==
==   Мячик -- самый умный элемент в игре, ему пригодятся скорость и уско-==
== рение, а также мифический коэффициент притягивания. Имеет потомков,   ==
== которые называются бонусами. /                                        ==
==                                                                       ==
==   Ball -- ball or bonus.                                              ==
==   Description:                                                        ==
==   Its flying upon the field, its real, its real.                      ==
=========================================================================*/
/*Авторы/Authors: pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь
                       zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь*/
#ifndef BALL_H
#define BALL_H

#include <QGraphicsItem> //базовый класс, графический элемент qt
#include <QPointF> //точка
#include "field.h" //поле

class Ball : public QGraphicsItem
{
  double radius_; //радиус мячика
  QPointF point_;
  Field *field_;
  bool painted_;
public:
    Ball(Field*,QGraphicsItem *parent=0);
    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *,QWidget*);
    void timerEvent(QTimerEvent *);
    QPainterPath shape() const;
    void moveMe(); //движение мячика по таймеру / moving ball on timer
};

#endif // BALL_H
