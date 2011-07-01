/*=========================================================================
==                          player.h                                     ==
==   Player -- играющий человек, способен управлять вагонеткой, класс    ==
== включает интерфейс обработки событий, действий, мозговых импульсов    ==
== пользователя. /                                                       ==
==                                                                       ==
==   Player -- class representing interactoins with people (users).      ==
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
#ifndef PLAYER_H
#define PLAYER_H
#include "gamer.h" //родитель

class Player : public Gamer
{
    static const double dang; //изменение угла / delta angle
public:
    Player(Field*,int);
    //переопределение функций Игрока / redefinition of Gamer's functions
    void mouseMoved(QPointF&); //движение мыши
    void mousePress(QPointF&); //нажатие на кнопку мыши
    void keyPressed(int); //нажатие на стрелку
};

#endif // PLAYER_H
