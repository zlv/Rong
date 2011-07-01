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
#include <QPainter>
#include <QPixmap>
#include <math.h>
#include "platform.h"
#include "constants.h"

/*эталонный радиус окружности смерти; радиус круга смерти сравнивается
с этой константой, и увеличивается или уменьшается /
  radius of the circle of death, that affects on the platform size*/
const double Platform::neededRadius = 250;

Platform::Platform(int i, QGraphicsItem *parent) :
    QGraphicsItem(parent), index_(i),
    pixmapSize_(120,20), pixmap_(pixmapSize_.width(),pixmapSize_.height()),
    circle_(static_cast<CircleOfDeath*>(parent))
{
    //минимальный угол, на него влияет размер ограничивающей области
    minAngle_ = circle_->limiter(i)+CircleOfDeath::limitEmptySpace;
    //максимальный, то же самое
    maxAngle_ = circle_->limiter(i)
            +fabs(circle_->limiter(i+1)-circle_->limiter(i))
            -CircleOfDeath::limitEmptySpace;
    //текущий угол вычисляется как среднее
    angle_ = (minAngle_+maxAngle_)/2;
    //все точки вагонетки
    polygon_ << QPointF(0,0)
             << QPointF(pixmapSize_.height()-1,pixmapSize_.height()-1)
             << QPointF(pixmapSize_.width()-pixmapSize_.height()-1,
                        pixmapSize_.height()-1)
             << QPointF(pixmapSize_.width()-1,0);
    if (!bPixmap) return; //дальше рисование битмапа
    pixmap_.fill(Qt::transparent);
    QPainter* painter = new QPainter(&pixmap_);
    QBrush brush(Qt::SolidLine);
    if (i==0) //цвет в зависимости от номера вагонетки
        brush.setColor(Qt::darkRed);
    else
        brush.setColor(Qt::darkBlue);
    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(polygon_);
    delete painter;
}

/*================================
===    Унаследованные функции    ===
  ================================*/

QRectF Platform::boundingRect() const //регион отсечения
{
    QPolygonF polygon = findPolygon(); //найти все точки
    double width=-1,height=-1; //ширина,высота
    double leftx=INT_MAX,lefty=INT_MAX; //крайние левая и правая точки

    for (int i=0; i<4; i++)
    {
        //найти крайнюю точку
        if (polygon[i].x()<leftx)
            leftx = polygon[i].x();
        if (polygon[i].y()<lefty)
            lefty = polygon[i].y();
        //найти наибольший габарит
        for (int j=i+1; j<4; j++)
        {
            double twidth = fabs(polygon[j].x()-polygon[i].x());
            double theight= fabs(polygon[j].y()-polygon[i].y());
            if (width<twidth)
                width = twidth;
            if (height<theight)
                height = theight;
        }
    }

    return QRectF(leftx,lefty,width,height);
}

//форма фигуры (для сравнивания)
QPainterPath Platform::shape() const
{
    QPolygonF polygon = findPolygon(); //найти точки
    polygon << polygon[0]; //соединяющая первую точка (на всякий случай)
    QPainterPath path;
    path.addPolygon(polygon);
    return path;
}

void Platform::paint(QPainter *p, const QStyleOptionGraphicsItem *,
                     QWidget *)
{
    double radius = circle_->radius(); //радиус круга смерти
    double max = radius/neededRadius; //увеличение

    p->rotate(90.0+angle_/PI*180); // поворот
    if (!bPixmap) //рисование без битмапа
    {
        p->setRenderHint(QPainter::Antialiasing,true); //сглаживание
        QBrush brush(Qt::SolidLine);
        if (index_==0)
            brush.setColor(Qt::darkRed);
        else
            brush.setColor(Qt::darkBlue);
        p->setBrush(brush);
        QPolygonF polygon = polygon_;
        for (int i=0; i<4; i++)
        {
            polygon[i] = polygon[i]*max
                    +QPointF(-pixmapSize_.width()/2*max,
                             -pixmapSize_.height()/2*max-radius);
        }
        p->drawPolygon(polygon);
    }
    else //с битмапом
    {
        p->setRenderHint(QPainter::SmoothPixmapTransform,true);
        QRectF target(-pixmapSize_.width()/2*max,
                      -pixmapSize_.height()/2*max-radius,
                      pixmapSize_.width()*max,pixmapSize_.height()*max);
        QRectF source(0,0,pixmapSize_.width(),pixmapSize_.height());
        p->drawPixmap(target,pixmap_,source);
    }
}

/*=================
===     Слоты     ===
  =================*/

//изменение угла (смотря на точку)
void Platform::changeAngle(QPointF p)
{
    double tempAngle; //вычислить угол
    tempAngle = acos(p.x()/sqrt(p.y()*p.y()+p.x()*p.x()));
    tempAngle = index_?circle_->limiter(index_)+PI-tempAngle:
                circle_->limiter(index_)+tempAngle;
    //проверка на границы
    if (tempAngle<=maxAngle_ && tempAngle>=minAngle_)
    {
        angle_ = tempAngle;
        emit update();
    }
}

//изменение угла (только прибавить изменение)
void Platform::changeAngle(double da)
{
    double tempAngle;
    tempAngle = angle_+da;
    //проверка на границы
    if (tempAngle<=maxAngle_ && tempAngle>=minAngle_)
    {
        angle_ = tempAngle;
        emit update();
    }
}

//найти нужный нам многоугольник
QPolygonF Platform::findPolygon() const
{
    double radius = circle_->radius(); //радиус круга смерти
    double max = radius/neededRadius; //увеличение
    double width = pixmapSize_.width() *max/2; //пол ширины вагонетки
    double height= pixmapSize_.height()*max/2; //пол высоты вагонетки
    //проекция высот на оси
    double wx = width*sin(angle_)-height*cos(angle_);
    double wy = width*cos(angle_)+height*sin(angle_);
    //теперь нужно изменять каждую точку
    QPolygonF polygon = polygon_;
    for (int i=0; i<4; i++)
    {
        double angle = angle_+PI;
        /*расстояние от (0;0) до точки /
          length of line between (0;0) and point*/
        double length = sqrt(pow(polygon[i].x(),2)+pow(polygon[i].y(),2));
        //повернуть точку / rotate point
        if (length==0)
            polygon[i] = QPointF(0,0);
        else
            polygon[i] = QPointF(length*cos(angle+asin(polygon[i].x()/length)),
                                 length*sin(angle+asin(polygon[i].x()/length)))*max;
        //переместить точку / move point
        polygon[i]+= QPointF(-wx+radius*cos(angle_),
                             +wy+radius*sin(angle_));
    }

    return polygon;
}
