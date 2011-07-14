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
#include "gamer.h"
#include "constants.h"
#include "circleofdeath.h"
#include "platform.h"

/*====================================*
 *====================================*
 *                Gamer               *
 *====================================*
 *====================================*/

const double Gamer::dang=PI/120; //изменение угла / delta angle

Gamer::Gamer(Field* f, int p, Type t) : platform_(p), type_(t),
    listening_(0), field_(f), controls_(MousePressControl), stopped_(0){}

/*================================
====      Открытые функции      ====
  ================================*/

void Gamer::stop() //остановить поток
{
    stopped_ = 1;
}

void Gamer::setControls(Gamer::Controls c) //указать управление
{
    controls_ = c;
    switch (c)
    {
    case MousePressControl:
        listening_ = mousePressListening;
        break;
    case MouseMoveControl:
        listening_ = mouseMovedListening;
        break;
    case KeyrdNumbsAndArrows:
        listening_ = keyrdNumbsListening+keyrdArrowListening
                    +keyrdMovegListening;
        break;
    case KeyrdNumbsAndArrowsSpec:
        listening_ = keyrdNumbsListening+keyrdArrowListening
                    +keyrdSpeclListening;
        break;
    case KeyrdLetrsOnly:
        listening_ = keyrdAnumsListening+keyrdWasdkListening
                    +keyrdMovegListening;
        break;
    case KeyrdLetrsOnlySpec:
        listening_ = keyrdAnumsListening+keyrdWasdkListening
                    +keyrdSpeclListening;
        break;
    }
}

//пустышки
void Gamer::mouseMoved(QPointF&){}

void Gamer::mousePress(QPointF&){}

void Gamer::keyPressed(int){}

void Gamer::timerTickd(){}

void Gamer::decCursorTime(){}

void Gamer::setMouseMovedListening() //слушать ли движение мыши
{
    listening_+=mouseMovedListening;
}

void Gamer::setMousePressListening() //слушать ли нажатия кнопок мыши
{
    listening_+=mousePressListening;
}

void Gamer::setKeyrdArrowListening() //слушать ли стрелки
{
    listening_+=keyrdArrowListening;
}

void Gamer::setTimerTickdListening() //слушать ли часы
{
    listening_+=timerTickdListening;
}

Gamer::Type Gamer::type(){return type_;} //вернуть тип

Gamer::Controls Gamer::controls() //какое управление
{
    return controls_;
}

//изменение переменных направления
void Gamer::changeDirection(FieldData&){}

/*==========================================
====    Функция, оставленная потомкам     ====
  ==========================================*/

void Gamer::checkRedo()
{
    CircleOfDeath *circle = field_->circle();
    bool crossOver = circle->platform( platform_)->collidesWithItem(
                circle->platform(!platform_));
    if  (crossOver)  circle->platform( platform_)->redoAngle();
}

/*====================================*
 *====================================*
 *             FieldData              *
 *====================================*
 *====================================*/

//инициализация без скорости и радиуса
FieldData::FieldData(QPointF p, double a1, double a2,
                     double l1, double l2)
    : ballPoint(p)
{
    platformAngle[0] = a1;
    platformAngle[1] = a2;
    platformLimiter[0] = l1;
    platformLimiter[1] = l2;
}

//инициализация со скоростью и радиусом
FieldData::FieldData(QPointF p, double a1, double a2,
                     double l1, double l2,
                     double x,  double y,  double r)
    : ballPoint(p), vx(x), vy(y), circleRadius(r)
{
    platformAngle[0] = a1;
    platformAngle[1] = a2;
    platformLimiter[0] = l1;
    platformLimiter[1] = l2;
}
