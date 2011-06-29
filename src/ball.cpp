#include "ball.h"
#include <QPainter>
#include <QWidget>
#include <math.h>
#include <QDebug>
#include "constants.h"
#include "func.h"
Ball::Ball(Field *f, QGraphicsItem *parent)  :
    QGraphicsItem
    (parent), radius_(10), point_(0,240), field_(f), painted_(0)
{
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
    QBrush br;
    QPen pen;
    pen.setStyle(Qt::NoPen);
    br.setColor(Qt::blue);
    br.setStyle(Qt::SolidPattern);
    p->setRenderHint(QPainter::Antialiasing,true);
    p->setBrush(br);
    p->setPen(pen);
    p->drawEllipse(point_,static_cast<int>(radius_),
                          static_cast<int>(radius_));
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

    static double x=0.3,y=0.1;

    if (!collidesWithItem(field_->circle(),Qt::ContainsItemShape))
    {
        double arc=acos(point_.y()/(sqrt(point_.x()*point_.x()+point_.y()*point_.y())));
        arc=(arc>PI/2?PI-arc:arc);
       // qDebug()<<point_.x()<<' '<<point_.y()<<' '<<arc*180/PI;
        if (point_.x()>0 && point_.y()>=0)
        {
            double bx = x;
            double by = y;
            double length = sqrt(x*x+y*y);
            double angle;
            if (bx>=0)
            {
               angle=acos(bx/sqrt(bx*bx+by*by))+2*arc;
               x = length*cos(angle);
               if(y<0){
                  y =-length*sin(angle);
               }
               else
               {
               y =-length*sin(angle);
               }

               qDebug()<<x<<' '<<y<<' '<<arc*180/PI;
            }
            else if (bx<0)
            {
             //  angle = -fallAngle+tangentAngle;

              angle=acos(-bx/sqrt(bx*bx+by*by))-arc;
                qDebug()<<angle*180/PI;
                x =-length*cos(angle);
                y =-length*sin(angle);
            }
        }
        else if (point_.x()>0 && point_.y()<0)
        {
            double bx = x;
            double by = y;
            double angle;
            double length = sqrt(x*x+y*y);
            if (bx>=0)
            {
          //      angle = -fallAngle-tangentAngle;
                angle=acos(-bx/sqrt(bx*bx+by*by))+2*arc;
                x = length*cos(angle);
                y =-length*sin(angle);
            }
            else if (bx<0)
            {
             //   angle = fallAngle+tangentAngle;
                angle=PI-acos(bx/sqrt(bx*bx+by*by))-arc;
                x = length*cos(angle);
                y =-length*sin(angle);
            }
        }
        else
        {
            x=0;
            y=0;
        }
    }

    point_=QPointF(point_.x()+x,point_.y()+y);
}
