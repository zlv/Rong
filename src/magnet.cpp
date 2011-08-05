/*Авторы/Authors: pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь
                       zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь*/
#include "magnet.h"
#include <QPainter>
#include <QWidget>
#include <math.h>
#include <QDebug>
#include "constants.h"
#include "circleofdeath.h"
#include "platform.h"
#include "score.h"
Magnet::Magnet(Field *f,QString sImg, QGraphicsItem *parent)
    : QGraphicsItem(parent), radius_(30), point_(0,0),
    field_(f), pixmap_(sImg), painted_(0),
    showed_(0)
{
    pixmap_=QPixmap(sImg);
}

QRectF Magnet::boundingRect() const
{
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius; //увеличение
    return QRectF(point_.x()-radius_*max, point_.y()-radius_*max,
                   radius_*2*max,radius_*2*max);
}

QPainterPath Magnet::shape() const
{
    QPainterPath path;
    double radius = field_->circle()->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius; //увеличение
    path.addEllipse(point_.x()-radius_*max, point_.y()-radius_*max,
                    radius_*2*max,radius_*2*max);
    return path;
}

void Magnet::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
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

QPointF Magnet::point()
{
    return point_;
}

void Magnet::show()
{
    point_ = QPoint(0,0);
    showed_ = 1;
}

bool Magnet::showed()
{
     return showed_;
}

void Magnet::hide()
{
    showed_ = 0;
}

void Magnet::drawBall()
{
    pixmap_.fill(Qt::transparent);
    QPainter* p = new QPainter(&pixmap_);
    QPen pen;
    pen.setStyle(Qt::NoPen);
    p->setPen(pen);
    p->setRenderHint(QPainter::Antialiasing,true);
    p->drawEllipse(QPointF(radius_,radius_),static_cast<int>(radius_),
                          static_cast<int>(radius_));
    delete p;
}

void Magnet::setImage(QString sImg)
{
    pixmap_ = QPixmap(sImg);
}
