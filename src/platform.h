/*=========================================================================
==                           platform.h                                  ==
==   Platform -- Вагонетка, объект, перемещающийся по кругу смерти и от- ==
== бивающий мячи. Может иметь разные цвета[, размеры].  /                ==
==                                                                       ==
==   Platform -- platform for playing, for pushing ball, for             ==
==  entertaining.                                                        ==
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
#ifndef PLATFORM_H
#define PLATFORM_H
#include <QGraphicsItem>

class CircleOfDeath; //мы размещаемcя на круге смерти / we are in it

class Platform : public QGraphicsItem
{
    //номер вагонетки на круге смерти /
    int index_; //number of platform in the circle of death
    //минимальный,максимальный,текущий,предтекущий угол
    double minAngle_,maxAngle_,angle_,lastAngle_;
    QSizeF pixmapSize_; //размер изображения платформы
    QPixmap pixmap_; //изображение платформы
    QPolygonF polygon_; //точки вагонетки
    CircleOfDeath *circle_; //мы находимся на круге
    double bonusMax_; //бонусное увеличение / maximize by bonus
    //1 -- рисовать битмап, 0 -- рисовать линиями
    bool bPixmap_;
    //имена файлов с изображениями вагонеток / platform images
    static const std::string filenames[2];

public:
    Platform(int, QGraphicsItem *parent = 0);
    //унаследованные функции
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *,QWidget*);
    double angle(); //угол в круге
    double angleTox(); //угол к оси x (от 0 до pi/2)
    void redoAngle(); //отменить изменение угла
    void setAngle(double); //установить угол
    void incSize(); //увеличить размер вагонетки
    void decSize(); //уменьшить размер вагонетки
    //сделать размер платформы прежним / return to a normal state
    void returnPlatformSize();
public slots:
    void changeAngle(QPointF); //изменение угла (смотря на точку)
    void changeAngle(double);//изменение угла (только прибавить изменение)
private:
    QPolygonF findPolygon() const; //найти нужный нам многоугольник
    bool checkAngle(double); //проверить угол
};

#endif // PLATFORM_H
