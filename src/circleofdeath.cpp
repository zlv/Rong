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
#include "constants.h" //Пи
#include "platform.h" //вагонетки
#include "ball.h" //мяч нужен для определения контуров
#include <QPainter> //рисование
#include <QWidget>  //размеры окна
#include <math.h>

const double CircleOfDeath::limitEmptySpace = PI/36; //ширина ограничителей

/*эталонный радиус окружности смерти; радиус круга смерти сравнивается
с этой константой, и увеличивается или уменьшается /
  radius of the circle of death, that affects on the platform size*/
const double CircleOfDeath::neededRadius = 250;

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
    if  (Field::debug)
    {
        pen.setWidth(1);
        p->setPen(pen);
        p->drawRect(platform(0)->boundingRect());
        p->drawRect(platform(1)->boundingRect());
        p->drawRect(boundingRect());
        p->drawRect(field_->ball()->boundingRect());
        p->drawPath(platform(0)->shape());
        p->drawPath(platform(1)->shape());
        p->drawPath(shape());
        p->drawPath(field_->ball()->shape());
    }
}

/*================================
====      Открытые функции      ====
  ================================*/

double CircleOfDeath::radius() //радиус круга
{
    return radius_;
}

double CircleOfDeath::limiter(int i) const //его итый ограничетель
{
    return limiter_[i];
}

Platform * CircleOfDeath::platform(int i) //его итая вагонетка
{
    return platform_[i];
}

//вернуть номер вагонетки по одной точке
int CircleOfDeath::getColor(QPointF& p)
{
    double angle = acos(p.x()/sqrt(p.x()*p.x()+p.y()*p.y()));
    if (p.y()<0) angle = 2*PI-angle;
    if (angle<limiter_[1] && angle>limiter_[0])
        return 0;
    else
        return 1;
}
