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

const double Gamer::dang=PI/144; //изменение угла / delta angle

/*================================
====      Открытые функции      ====
  ================================*/

Gamer::Gamer(Field* f, int p, Type t) : platform_(p), type_(t),
    listening_(0), field_(f){}

//пустышки
void Gamer::mouseMoved(QPointF&){}

void Gamer::mousePress(QPointF&){}

void Gamer::keyPressed(int){}

void Gamer::timerTickd(int){}

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

//изменение переменных направления
void Gamer::changeDirection(QPointF&,double){}
