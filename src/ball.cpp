/*
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
#include "ball.h"
#include <QPainter>
#include <QWidget>
#include <math.h>
#include <QDebug>
#include "constants.h"
#include "circleofdeath.h"
Ball::Ball(Field *f, QGraphicsItem *parent)  :
    QGraphicsItem
    (parent), radius_(10), point_(0,0),
    field_(f), pixmap_(22,22), painted_(0)
{

    pixmap_.fill(Qt::transparent);
    QPainter* p = new QPainter(&pixmap_);
    QBrush br;
    QPen pen;
    pen.setStyle(Qt::NoPen);
    br.setColor(Qt::blue);
    br.setStyle(Qt::SolidPattern);
    p->setPen(pen);
    p->setBrush(br);
    p->setRenderHint(QPainter::Antialiasing,true);
    p->drawEllipse(QPointF(radius_,radius_),static_cast<int>(radius_),
                          static_cast<int>(radius_));
    delete p;
}

/*================================
===    Унаследованные функции    ===
  ================================*/

QRectF Ball::boundingRect() const //регион отсечения
{
    return QRectF(-radius_, -radius_,
                   radius_*2,radius_*2);
}

//рисование мячика
void Ball::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget*)
{

    p->drawPixmap(QPoint(point_.x()-radius_,point_.y()-radius_),pixmap_);
    painted_ = 1;
}

//форма фигуры (для сравнивания)
QPainterPath Ball::shape() const
{
    QPainterPath path;
    path.addEllipse(point_,static_cast<int>(radius_),
                           static_cast<int>(radius_));
    return path;
}

/*================================
====      Открытая функция      ====
  ================================*/

//движение мячика по таймеру / moving ball on timer
void Ball::moveMe()
{
    if (!painted_) return;
    //проекции оси движения на оси X и Y
    static double x=0.8,y=0.9;
    //если перетаёт пересекаться с кругом смерти
    if (!collidesWithItem(field_->circle(),Qt::ContainsItemShape))
    {
        //модуль вектора движения(равен модулю векторов отражения и падения)
        double length = sqrt(x*x+y*y);
        //угол падающего вектора к оси X
        double angle;
        //угол касательной к окружности в точке соприкосновения
        double arc=acos(point_.y()/(sqrt(point_.x()*point_.x()+point_.y()*point_.y())));
        //который не больше 90
        arc=(arc>PI/2?PI-arc:arc);
        //правая-нижняя часть круга смерти
        if (point_.x()>0 && point_.y()>=0)
        {
            //если движется вправо
            if (x>=0)
            {
                //угол вектора отражения к оси X
                angle=acos(x/length)+2*arc;
                //проекция отраженного вектора на ось X
                x = length*cos(angle);
                //если движется вниз
                if(y<0)
                {
                   //угол вектора отражения к оси X
                   angle=acos(x/length)+arc;
                }
                //проекция отраженного вектора на ось Y
                y =-length*sin(angle);
            }
            //если движется влево
            else
            {
                //угол вектора отражения к оси X
                angle=acos(-x/length)-arc;
                //проекция отраженного вектора на ось X
                x =-length*cos(angle);
                //проекция отраженного вектора на ось Y
                y =-length*sin(angle);
            }
        }
        else
            //правая верхняя часть круга смерти
            if (point_.x()>0 && point_.y()<0)
            {
                if (x>=0)
                {
                    if(y<0)
                    {
                        angle=2*arc+acos(x/length);
                        x =length*cos(angle);
                        y =length*sin(angle);
                    }
                    else
                    {
                        angle=PI/2+acos(x/length)-2*arc;
                        x =length*sin(angle);
                        y =length*cos(angle);
                    }

                }
                else
                {
                    if(y<0)
                    {
                        angle=arc-acos(-x/length);
                        x =-length*cos(angle);
                        y =-length*sin(angle);
                    }
                    else
                    {
                        angle=acos(y/length)-arc;
                        x =-length*cos(angle);
                        y =-length*sin(angle);
                    }

                }
            }
            else
                //левая верхняя часть круга смерти
                if (point_.x()<0 && point_.y()<0)
                {
                    if (x>=0)
                    {
                        if(y<0)
                        {
                            angle=arc-acos(x/length);
                            x = length*cos(angle);
                            y =-length*sin(angle);
                        }
                        else
                        {
                            angle=acos(x/length)-arc;
                            x =length*cos(angle);
                            y =-length*sin(angle);
                        }

                    }
                    else
                    {
                        if(y<0)
                        {
                            angle=-acos(-y/length)-PI/2+2*arc;
                            x =length*cos(angle);
                            y =-length*sin(angle);
                        }
                        else
                        {
                            angle=-acos(-x/length)+2*arc;
                            x =-length*cos(angle);
                            y =length*sin(angle);
                        }

                    }
                }
                else
                    //левая нижняя часть круга смерти
                    if (point_.x()<0 && point_.y()>0)
                    {
                        if (x>=0)
                        {
                            if(y<0)
                            {
                                angle=acos(x/length);
                                x =length*cos(angle);
                                y =length*sin(angle);
                            }
                            else
                            {
                                angle=acos(x/length)-arc;
                                x =length*cos(angle);
                                y =-length*sin(angle);
                            }

                        }
                        else
                        {
                            if(y<0)
                            {
                                angle=2*arc-acos(-x/length);
                                x =-length*cos(angle);
                                y =-length*sin(angle);
                            }
                            else
                            {
                                angle=PI-acos(-x/length)-2*arc;
                                x =length*cos(angle);
                                y =-length*sin(angle);
                            }

                        }
                    }
    }
    //изменение координат летающего круга в соответствии с вектором движения
    point_=QPointF(point_.x()+x,point_.y()+y);
}
