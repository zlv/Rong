/*=========================================================================
==                           gamer.h                                     ==
==   Gamer -- играющее существо (бот или человек), способно управлять по-==
== лем (точнее одной из вагонеток на нём. /                              ==
==                                                                       ==
==   Gamer -- class for control platforms (it can be bot or human).      ==
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
#ifndef GAMER_H
#define GAMER_H
#include <QThread> //любой объект этого вида может думать в отдельном потоке
#include <QPointF>
#include "field.h" //поле

class Gamer : public QThread
{
    Q_OBJECT

protected:
    static const double dang; //изменение угла / delta angle
    int platform_; //номер вагонетки, связанной с нами / number of platform
    enum Type{Human, AI}type_; //кто ты? ангел или бес
    static const int mouseMovedListening=0x1; //слушать мышь
    static const int mousePressListening=0x2; //слушать нажатия мыши
    static const int keyrdArrowListening=0x4; //слушать стрелки
    static const int timerTickdListening=0x8; //слушать
    int listening_; //что мы слушаем
    Field *field_; //связывающее всех поле

public:
    Gamer(Field*,int,Type);
    virtual void mouseMoved(QPointF&); //движение мыши
    virtual void mousePress(QPointF&); //нажатие на кнопку мыши
    virtual void keyPressed(int); //нажатие на стрелку
    virtual void timerTickd(int); //прошло время
    void setMouseMovedListening(); //слушать ли движение мыши
    void setMousePressListening(); //слушать ли нажатия кнопок мыши
    void setKeyrdArrowListening(); //слушать ли стрелки
    void setTimerTickdListening(); //слушать ли время
public slots:
    //изменить переменные
    virtual void changeDirection(QPointF&,double);
};

#endif // GAMER_H
