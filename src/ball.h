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
===========================================================================
==  Rong is free software: you can redistribute it and/or modify         ==
==  it under the terms of the GNU General Public License as published by ==
==  the Free Software Foundation, either version 3 of the License, or    ==
==  (at your option) any later version.                                  ==
==                                                                       ==
==  Rong is distributed in the hope that it will be useful,              ==
==  but WITHOUT ANY WARRANTY; without even the implied warranty of       ==
==  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        ==
==  GNU General Public License for more details.                         ==
==                                                                       ==
==  You should have received a copy of the GNU General Public License    ==
==  along with this program.  If not, see <http://www.gnu.org/licenses/>.==
==                                                                       ==
=========================================================================*/
/*Авторы/Authors: pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь
                       zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь*/
#ifndef BALL_H
#define BALL_H

#include <QGraphicsItem> //базовый класс, графический элемент qt
#include <QPointF> //точка
#include <QPixmap>
#include "field.h" //поле

class Ball : public QGraphicsItem
{
protected:
  double radius_; //радиус мячика
  QPointF point_;
  Field *field_;
  QPixmap pixmap_;
  bool painted_;
  enum BallColor{RED,BLUE,WHITE} color_; // цвет мячика
  double vx_; //проекция вектора движения на ось X
  double vy_; //проекция вектора движения на ось Y
  bool showed_;
public:
  enum Type{GameBall,BonusBall} type;

  Ball(Field*,double,QGraphicsItem *parent=0);
  Ball(Field*,double,Type,QString,QGraphicsItem *parent=0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *,QWidget*);
    void moveMe(); //движение мячика по таймеру / moving ball on timer
    void mirror(double arc); // рассчёт изменения вектора при отражении
    QPointF point();
    double vx(); // скорость по X
    double vy();// скорость по Y
    double velocity(); // скорость
    double color();
    void setVelocity(double);
    void setVelocity(double,double);
    void show();
    bool showed();
    void hide();
    void renew();
protected:
    void setRandomAngleVelocity(double);
    void changeColor();
    void drawBall();
    void setImage(QString);
};

#endif // BALL_H
