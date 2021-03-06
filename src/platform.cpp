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
#include "platform.h"
#include "constants.h"
#include "circleofdeath.h"
#include <QPainter>
#include <QPixmap>
#include <math.h>
#include <limits.h>

//имена файлов с изображениями вагонеток / platform images
const std::string Platform::filenames[2] = {":/images/platform-red.png",
                                            ":/images/platform-blu.png"};

//i -- index of platform
Platform::Platform(int i, QGraphicsItem *parent) :
    QGraphicsItem(parent), index_(i),
    pixmapSize_(120,20), pixmap_(QString::fromStdString(filenames[i])),
    circle_(static_cast<CircleOfDeath*>(parent)),
    bonusMax_(1.0)
{
    bPixmap_ = !pixmap_.isNull();
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
             << QPointF(pixmapSize_.height()/2-1,pixmapSize_.height()-1)
             << QPointF(pixmapSize_.width()-pixmapSize_.height()/2-1,
                        pixmapSize_.height()-1)
             << QPointF(pixmapSize_.width()-1,0);
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
                width=twidth;
            if (height<theight)
                height=theight;
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

//рисование само сабой
void Platform::paint(QPainter *p, const QStyleOptionGraphicsItem *,
                     QWidget *)
{
    double radius = circle_->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius*bonusMax_; //увеличение

    p->rotate(90.0+angle_/PI*180); // поворот
    if (!bPixmap_) //рисование без битмапа
    {
        p->setRenderHint(QPainter::Antialiasing,true); //сглаживание
        QBrush brush(Qt::SolidLine);
        if (index_==0) //цвет вагонетки
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
        //вывести с увеличением
        QRectF target(-pixmapSize_.width()/2*max,
                      -pixmapSize_.height()/2*max-radius,
                      pixmapSize_.width()*max,pixmapSize_.height()*max);
        QRectF source(0,0,pixmapSize_.width(),pixmapSize_.height());
        p->drawPixmap(target,pixmap_,source);
    }
}

/*================================
===      общедоступные функции   ===
  ================================*/

double Platform::angle() //угол в круге
{
    return angle_;
}

double Platform::angleTox() //угол к оси x (от 0 до pi/2)
{
    QPolygonF polygon = findPolygon();
    double dx = polygon[2].x()-polygon[1].x();
    double dy = polygon[2].y()-polygon[1].y();
    return acos(fabs(dx)/sqrt(dx*dx+dy*dy));
}

void Platform::redoAngle() //отменить изменение угла
{
    angle_ = lastAngle_;
}

void Platform::setAngle(double a) //установить угол
{
    lastAngle_ = angle_;
    angle_ = a;
    emit update();
}

//увеличить размер вагонетки
void Platform::incSize()
{
    bonusMax_ = 2.0;
    emit update();
}

//уменьшить размер вагонетки
void Platform::decSize()
{
    bonusMax_ = 0.5;
    emit update();
}

//сделать размер платформы прежним / return to a normal state
void Platform::returnPlatformSize()
{
    bonusMax_ = 1;
    emit update();
}

/*=================
===     Слоты     ===
  =================*/

//изменение угла (смотря на точку)
void Platform::changeAngle(QPointF p)
{
    double tempAngle; //вычислить угол
    tempAngle = atan(p.x()/p.y())+PI/2;
    if (p.y()>0) tempAngle = PI-tempAngle;
    tempAngle = index_?circle_->limiter(index_)+PI-tempAngle:
                circle_->limiter(index_)+tempAngle;
    //проверка на границы
    if (checkAngle(tempAngle))
    {
        lastAngle_ = angle_;
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
    if (checkAngle(tempAngle))
    {
        lastAngle_ = angle_;
        angle_ = tempAngle;
        emit update();
    }
}

/*================================
===        ===========         ===
===       ==         ==        ===
===      ==  =======  ==       ===
===      ==  =======  ==       ===
===       ==         ==        ===
===        ===========         ===
==================================*/

//найти нужный нам многоугольник
QPolygonF Platform::findPolygon() const
{
    double radius = circle_->radius(); //радиус круга смерти
    double max = radius/CircleOfDeath::neededRadius*bonusMax_; //увеличение
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

bool Platform::checkAngle(double a) //проверить угол
{
    return a<=maxAngle_ && a>=minAngle_;
}
