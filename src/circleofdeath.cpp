/*=========================================================================
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
#include "circleofdeath.h" //этот класс
#include <QPainter> //рисование
#include <QWidget>  //размеры окна
#include "constants.h" //Пи
#include "platform.h" //вагонетки

const double CircleOfDeath::limitEmptySpace = PI/36; //ширина ограничителей

CircleOfDeath::CircleOfDeath(Field *f, QGraphicsItem *parent) :
    QGraphicsItem(parent), radius_(0), field_(f)
{
    for (int i=0; i<2; i++) //установка ограничений и платформ
        limiter_[i] = PI*i; //ограничетели через каждую Пи
    for (int i=0; i<2; i++) //создание двух вагонеток
        platform_[i] = new Platform(i,this);
}

/*================================
===    Унаследованные функции    ===
  ================================*/

QRectF CircleOfDeath::boundingRect() const //регион отсечения
{
    qreal penWidth = 1;
    return QRectF(-radius_-penWidth/2,-radius_-penWidth/2,
                   radius_*2+penWidth, radius_*2+penWidth);
}

//форма фигуры (для сравнивания)
QPainterPath CircleOfDeath::shape() const
{
    QPainterPath path;
    path.addEllipse(-radius_,-radius_,radius_*2,radius_*2);
    return path;
}

//рисование
void CircleOfDeath::paint(QPainter *p, const QStyleOptionGraphicsItem *,
                          QWidget *w)
{
    QSize size = w->size(); //размер окна / size of the window
    radius_ = size.width()<size.height()? //минимальный габарит
              size.width():size.height();
    radius_=radius_/2.2; //радиус равен половине окна + круг смерти должен
    int alpha[4];//углы //быть меньше, чем окно
    alpha[0] = (limiter_[0]+limitEmptySpace)/2/PI*5760;//градусная мера
    alpha[1] = (limiter_[1]-limitEmptySpace)/2/PI*5760;//1/16 градуса; пол-
    alpha[2] = (limiter_[1]+limitEmptySpace)/2/PI*5760;//ный угол оборота
    alpha[3] = (limiter_[0]-limitEmptySpace)/2/PI*5760;//равен 360*16=5760
    QColor colors[] = {Qt::blue, Qt::red}; //цвета полуокружностей

    p->setRenderHint(QPainter::Antialiasing,true); //включить сглаживание
    QPen pen;
    pen.setWidth(3); //установить ширину линии

    for (int i=0; i<2; i++) //рисование каждой полуокружности
    {
        pen.setColor(colors[i]);
        p->setPen(pen);
        p->drawArc(-radius_,-radius_,radius_*2,radius_*2,alpha[i*2],
                   (alpha[i*2+1]<0?(5760+alpha[i*2+1]):alpha[i*2+1])-
                    alpha[i*2]);
    }
}

/*================================
====      Открытые функции      ====
  ================================*/

double CircleOfDeath::radius() //радиус круга
{
    return radius_;
}

double CircleOfDeath::limiter(int i) //его итый ограничетель
{
    return limiter_[i];
}

Platform * CircleOfDeath::platform(int i) //его итая вагонетка
{
    return platform_[i];
}
