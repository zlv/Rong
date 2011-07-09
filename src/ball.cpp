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
#include "platform.h"
#include "score.h"
Ball::Ball(Field *f, double vx, double vy, QGraphicsItem *parent)  :
    QGraphicsItem
    (parent), radius_(30), point_(0,0),
    field_(f), pixmap_(62,62), painted_(0),
    color_(BLUE), vx_(vx),vy_(vy), type(GameBall)
{
    drawBall();
}

Ball::Ball(Field *f, double vx, double vy, Type t, QString& sImg, QGraphicsItem *parent)
    : QGraphicsItem(parent), radius_(32), point_(0,0),
    field_(f), pixmap_(sImg), painted_(0),
    color_(BLUE), vx_(vx), vy_(vy), type(t){}

/*================================
===    Унаследованные функции    ===
  ================================*/

QRectF Ball::boundingRect() const //регион отсечения
{
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius/4; //увеличение
    return QRectF(point_.x()-radius_, point_.y()-radius_,
                   radius_*2*max,radius_*2*max);
}

//форма фигуры (для сравнивания)
QPainterPath Ball::shape() const
{
    QPainterPath path;
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius/4; //увеличение
    path.addEllipse(point_.x()-radius_, point_.y()-radius_,
                    radius_*2*max,radius_*2*max);
    return path;
}

//рисование мячика
void Ball::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget*)
{
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius/4; //увеличение
    QRectF target(point_.x()-radius_,point_.y()-radius_,
                  radius_*2*max,radius_*2*max);
    QRectF source(0,0,radius_*2,radius_*2);
    p->drawPixmap(target,pixmap_,source);
    painted_ = 1;
}

/*================================
====      Открытая функция      ====
  ================================*/

//движение мячика по таймеру / moving ball on timer
void Ball::moveMe()
{
    if (!painted_) return;

    //проекции оси движения на оси X и Y
    //если перестаёт пересекаться с кругом смерти
    bool crossBorder = !collidesWithItem(field_->circle(),Qt::ContainsItemShape);
    bool crossPlatform[2];
    for (int i=0; i<2; i++)
    {
        crossPlatform[i] = collidesWithItem(field_->circle()->platform(i));
    }
    if (crossBorder || crossPlatform[0] || crossPlatform[1])
    {
        //модуль вектора движения(равен модулю векторов отражения и падения)
        //угол касательной к окружности в точке соприкосновения
        double arc;
        if(crossPlatform[0] || crossPlatform[1])
        {
            if(crossPlatform[0])
            {
                //смена цвета мяча
                if (color_==WHITE)
                {
                    color_ = RED;
                    drawBall();
                }
                else if (color_!=RED)
                {
                    changeColor();
                    color_ = RED;
                }
                arc=field_->circle()->platform(0)->angleTox();
            }
            else
            {
                //смена цвета мяча
                if (color_==WHITE)
                {
                    color_ = BLUE;
                    drawBall();
                }
                else if (color_!=BLUE)
                {
                    changeColor();
                    color_ = BLUE;
                }
                arc=field_->circle()->platform(1)->angleTox();
            }
        }
        else
        {
            arc=acos(point_.y()/(sqrt(point_.x()*point_.x()+point_.y()*point_.y())));
            BallColor circleColor = static_cast<BallColor>(field_->circle()->getColor(point_));
            //смена цвета мяча, а также реагирование на попадание не
            if (circleColor!=color_) //в свою часть круга смерти
            {
                if (color_==WHITE)
                {
                    color_ = circleColor;
                    drawBall();
                }
                else
                {
                    field_->score()->inc(color_);
                    color_ = WHITE;
                    drawBall();
                }
            }
        }
        //который не больше 90
        arc=(arc>PI/2?PI-arc:arc);
        qDebug()<<arc*180/PI;
        //правая-нижняя часть круга смерти
        mirror(arc);
    }
    //изменение координат летающего круга в соответствии с вектором движения
    point_=QPointF(point_.x()+vx_,point_.y()+vy_);
}


QPointF Ball::point()
{
    return point_;
}

void Ball::mirror(double arc)
{
    double length = sqrt(vx_*vx_+vy_*vy_);
    //угол падающего вектора к оси X
    double angle;
    if (point_.x()>0 && point_.y()>=0)
    {
        //если движется вправо
        if (vx_>=0)
        {
            if(vy_>0)//если движется вниз
            {
                //угол вектора отражения к оси X
                angle=acos(vx_/length)+2*arc;
                //проекция отраженного вектора на ось X
                vx_ = length*cos(angle);
                //проекция отраженного вектора на ось Y
                vy_ =-length*sin(angle);
            }
            else
            {
                angle=-PI/2+(PI-acos(vy_/length))+2*arc;
                qDebug()<<angle*180/PI<<' '<<(PI-acos(vy_/length))*180/PI<<' '<<arc*180/PI;
                //проекция отраженного вектора на ось X
                vx_ = length*cos(angle);
                //проекция отраженного вектора на ось Y
                vy_ = -length*sin(angle);

            }

        }
        //если движется влево
        else
        {
            if(vy_>0)
            {
            //угол вектора отражения к оси X
            angle=-(PI-acos(vx_/length))+2*arc;
            //проекция отраженного вектора на ось X
            vx_ =-length*cos(angle);
            //проекция отраженного вектора на ось Y
            vy_ =-length*sin(angle);
            }
            else
            {
                //угол вектора отражения к оси X
                angle=acos(vx_/length)+2*arc;
                //проекция отраженного вектора на ось X
                vx_ =-length*cos(angle);
                //проекция отраженного вектора на ось Y
                vy_ =-length*sin(angle);
            }

        }
    }
    else
        //правая верхняя часть круга смерти
        if (point_.x()>0 && point_.y()<0)
        {
            if (vx_>=0)
            {
                if(vy_<0)
                {
                    angle=2*arc+acos(vx_/length);
                    vx_ =length*cos(angle);
                    vy_ =length*sin(angle);
                }
                else
                {
                    angle=PI/2+acos(vx_/length)-2*arc;
                    vx_ =length*sin(angle);
                    vy_ =length*cos(angle);
                }

            }
            else
            {
                if(vy_<0)
                {
                    angle=arc-acos(-vx_/length);
                    vx_ =-length*cos(angle);
                    vy_ =-length*sin(angle);
                }
                else
                {
                    angle=acos(vy_/length)-arc;
                    vx_ =-length*cos(angle);
                    vy_ =-length*sin(angle);
                }

            }
        }
        else
            //левая верхняя часть круга смерти
            if (point_.x()<0 && point_.y()<0)
            {
                if (vx_>=0)
                {
                    if(vy_<0)
                    {
                        angle=arc-acos(vx_/length);
                        vx_ = length*cos(angle);
                        vy_ =-length*sin(angle);
                    }
                    else
                    {
                        angle=acos(vx_/length)-arc;
                        vx_ =length*cos(angle);
                        vy_ =-length*sin(angle);
                    }

                }
                else
                {
                    if(vy_<0)
                    {
                        angle=-acos(-vy_/length)-PI/2+2*arc;
                        vx_ =length*cos(angle);
                        vy_ =-length*sin(angle);
                    }
                    else
                    {
                        angle=-acos(-vx_/length)+2*arc;
                        vx_ =-length*cos(angle);
                        vy_ =length*sin(angle);
                    }

                }
            }
            else
                //левая нижняя часть круга смерти
                if (point_.x()<0 && point_.y()>0)
                {
                    if (vx_>=0)
                    {
                        if(vy_<0)
                        {
                            angle=acos(vx_/length)+2*arc;
                            vx_ =length*cos(angle);
                            vy_ =-length*sin(angle);
                        }
                        else
                        {
                            angle=-acos(vx_/length)+2*arc;
                            vx_ =length*cos(angle);
                            vy_ =length*sin(angle);
                        }

                    }
                    else
                    {
                        if(vy_<0)
                        {
                            angle=2*arc-acos(-vx_/length);
                            vx_ =-length*cos(angle);
                            vy_ =-length*sin(angle);
                        }
                        else
                        {
                            angle=PI-acos(-vx_/length)-2*arc;
                            vx_ =length*cos(angle);
                            vy_ =-length*sin(angle);
                        }

                    }
                }
}

//изменить цвет мяча
void Ball::changeColor()
{
    if (color_==BLUE)
        color_ = RED;
    else if (color_== RED)
        color_ =BLUE;
    drawBall();
}

//нарисовать мячик с новым цветом
void Ball::drawBall()
{
    pixmap_.fill(Qt::transparent);
    QPainter* p = new QPainter(&pixmap_);
    QBrush br;
    QPen pen;
    pen.setStyle(Qt::NoPen);
    if (color_==RED)
        br.setColor(Qt::red);
    else if (color_==BLUE)
        br.setColor(Qt::blue);
    else
        br.setColor(Qt::gray);
    br.setStyle(Qt::SolidPattern);
    p->setPen(pen);
    p->setBrush(br);
    p->setRenderHint(QPainter::Antialiasing,true);
    p->drawEllipse(QPointF(radius_,radius_),static_cast<int>(radius_),
                          static_cast<int>(radius_));
    delete p;
}
