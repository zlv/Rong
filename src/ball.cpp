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
#include <magnet.h>
#include <QDebug>
#include "constants.h"
#include "circleofdeath.h"
#include "platform.h"
#include "score.h"
Ball::Ball(Field *f, double v, QGraphicsItem *parent)  :
    QGraphicsItem
    (parent), radius_(10), point_(0,0),
    field_(f), pixmap_(62,62), painted_(0),
    color_(WHITE), showed_(1), type(GameBall)
{
    setRandomAngleVelocity(v); //случайное направление
    drawBall();
}

Ball::Ball(Field *f, double v, Type t, QString sImg, QGraphicsItem *parent)
    : QGraphicsItem(parent), radius_(10), point_(0,0),
    field_(f), pixmap_(sImg), painted_(0),
    color_(WHITE), showed_(1), type(t)
{
    if(t==BonusBall)radius_*=3; //увеличение размера, если бонус
    setRandomAngleVelocity(v);
}

/*================================
===    Унаследованные функции    ===
  ================================*/

QRectF Ball::boundingRect() const //регион отсечения
{
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius; //увеличение
    return QRectF(point_.x()-radius_*max, point_.y()-radius_*max,
                   radius_*2*max,radius_*2*max);
}

//форма фигуры (для сравнивания)
QPainterPath Ball::shape() const
{
    QPainterPath path;
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius; //увеличение
    path.addEllipse(point_.x()-radius_*max, point_.y()-radius_*max,
                    radius_*2*max,radius_*2*max);
    return path;
}

//рисование мячика
void Ball::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget*)
{
    if (!showed_) return;
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius; //увеличение
    QRectF target(point_.x()-radius_*max,point_.y()-radius_*max,
                  radius_*2*max,radius_*2*max);
    QRectF source(0,0,radius_*2,radius_*2);
    p->drawPixmap(target,pixmap_,source);
    painted_ = 1;
}

/*================================
===   собственные   функции    ===
  ================================*/

//движение мячика по таймеру / moving ball on timer
void Ball::moveMe()
{
    if (!painted_ || !showed_) return;
    bool crossBorder = !collidesWithItem(field_->circle(),Qt::ContainsItemShape);//если перестаёт пересекаться с кругом смерти
    bool crossPlatform[2];//если пересекается с одной из вагонеток
    bool crossMagnet = collidesWithItem(field_->magnet_);
    for (int i=0; i<2; i++)//2 вагонетки
    {
        crossPlatform[i] = collidesWithItem(field_->circle()->platform(i));//пересекается ли с iтой вагонеткой
    }
    if (crossBorder || crossPlatform[0] || crossPlatform[1])//если коснулся вагонеток, или перестал касаться круга смерти
    {
        double arc;//угол прямой к оси X от которой отражается шарик
        if(crossPlatform[0] || crossPlatform[1])//если коснулся какой-либо вагонетки
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
            //угол касательной к окружности, в точке, где шарик перестаёт пересекаться с кругом смерти
            arc=acos(point_.y()/(sqrt(point_.x()*point_.x()+point_.y()*point_.y())));
            BallColor circleColor = static_cast<BallColor>(field_->circle()->getColor(point_));
            //смена цвета мяча, а также реагирование на попадание не
            if (circleColor!=color_ && circleColor!=WHITE) //в свою часть круга смерти
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
        //угол к оси X прямой, от которой отражается шарик не больше 90
        arc=(arc>PI/2?PI-arc:arc);

        if(Field::debug)qDebug()<<arc*180/PI;
        //изменение вектора движения шарика
        mirror(arc);

    }
    if(field_->magnet_->showed()&&!crossMagnet)
         magnetMe();
    //изменение координат шарика в соответствии с вектором движения
    point_=QPointF(point_.x()+vx_,point_.y()+vy_);
}

//сказать в какой шарик точке
QPointF Ball::point()
{
    return point_;
}

//изменение вектора движения шарика
void Ball::mirror(double arc)
{
    //моуль вектора движения
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
            else//если движется вверх
            {
                //угол вектора отражения к оси X
                angle=-PI/2+(PI-acos(vy_/length))+2*arc;
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
            angle=(PI-acos(vx_/length))-2*arc;
            //проекция отраженного вектора на ось X
            vx_ =-length*cos(angle);
            //проекция отраженного вектора на ось Y
            vy_ =-length*sin(angle);
            }
            else
            {
             /*   //угол вектора отражения к оси X
                angle=acos(vx_/length)+2*arc;
                //проекция отраженного вектора на ось X
                vx_ =-length*cos(angle);
                //проекция отраженного вектора на ось Y
                vy_ =-length*sin(angle);*/
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
                            angle=PI+acos(vx_/length)-2*arc;
                            vx_ =-length*cos(angle);
                            vy_ =length*sin(angle);
                        }
                        else
                        {
                            angle=(PI-acos(vx_/length))+2*arc;
                            vx_ =-length*cos(angle);
                            vy_ =-length*sin(angle);

                        }

                    }
                }
}

double Ball::vx()
{
    return vx_;
}

double Ball::vy()
{
    return vy_;
}

void Ball::setRandomAngleVelocity(double v)
{
    double angle = PI*qrand()*2/RAND_MAX;
    setVelocity(v,angle);
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

double Ball::velocity() //скорость моя (значение)
{
    return sqrt(vx_*vx_+vy_*vy_);
}

void Ball::setVelocity(double v) //поставить скорость по модулю
{
    if (v<0) return;
    double angle = atan(vy_/vx_);
    if (vx_<0) angle+=PI;
    setVelocity(v,angle);
}

//поставить скорость по углу и модулю
void Ball::setVelocity(double v, double a)
{
    vx_ = cos(a)*v;
    vy_ = sin(a)*v;
}

void Ball::show() //разместить заново
{
    point_ = QPoint(0,0);
    showed_ = 1;
    setRandomAngleVelocity(sqrt(vx_*vx_+vy_*vy_));
}

bool Ball::showed() //вообще мяч есть?
{
    return showed_;
}

void Ball::hide()
{
    showed_ = 0;
}

void Ball::renew() //обнулить все значения
{
    color_ = WHITE;
    point_ = QPoint(0,0);
    setVelocity(velocity());
    show();
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
        br.setColor(Qt::darkGray);
    br.setStyle(Qt::SolidPattern);
    p->setPen(pen);
    p->setBrush(br);
    p->setRenderHint(QPainter::Antialiasing,true);
    p->drawEllipse(QPointF(radius_,radius_),static_cast<int>(radius_),
                          static_cast<int>(radius_));
    delete p;
}

void Ball::setImage(QString sImg)
{
    pixmap_ = QPixmap(sImg);
}

double Ball::color()
{
    return color_;
}

void Ball::magnetMe()
{
    chmag(point_.x(),1,0.5);
    chmag(point_.y(),0,0.8);
}

void Ball::chmag(double point, bool x, double power)
{
    double p=vy_,vel=velocity();
    if(x)
        p=vx_;
    if(point>0)
        p-=power;
    else
        p+=power;
    if(fabs(p)>fabs(vel))
    {
        if(p<0)
            p=vel*-1;
        else
            p=vel;
    }
    if(x)
        vx_=p;
    else
        vy_=p;
    p=sqrt(vel*vel-p*p);
    if(x)
        if(vy_<0)
           vy_=-1*p;
        else
           vy_=p;
    else
        if(vx_<0)
           vx_=-1*p;
        else
           vx_=p;
}


