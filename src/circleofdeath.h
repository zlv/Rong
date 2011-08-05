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
==                                                                       ==
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
/*Авторы/Authors: zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь
             pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь*/
#ifndef CIRCLEOFDEATH_H
#define CIRCLEOFDEATH_H
#include <QGraphicsItem> //базовый класс, графический элемент qt

class Field;
class Platform; //только объявление

class CircleOfDeath : public QGraphicsItem
{
    Platform *platform_[2]; //вагонетки
    double limiter_[2]; //углы ограничителей / limiters for circle of death
    double radius_; //радиус круга смерти / for the circle of death
    Field *field_; //связующее поле
    int alpha_[4];//углы
    //буквы, обозначающие клавишу, на которую нужно нажимать
    static const QString letters[9];
public:
    static const double limitEmptySpace; //ширина ограничителей
    /*эталонный радиус окружности смерти; радиус круга смерти сравнивается
    с этой константой, и платформа увеличивается или уменьшается /
      radius of the circle of death, that affects on the platform size*/
    static const double neededRadius;

    CircleOfDeath(Field *f, QGraphicsItem *parent = 0);
    //унаследованные функции
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter*,const QStyleOptionGraphicsItem *, QWidget*);
    //вернуть номер вагонетки по одной точке
    int getColor(QPointF&);
    //функции доступа
    double radius();
    double limiter(int) const;
    Platform* platform(int);
};

#endif // CIRCLEOFDEATH_H
