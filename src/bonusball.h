/*=========================================================================
==                             bonusball.h                               ==
==   BonusBall -- сын Ball'а, является мячиком, несущим в себе сюрприз,  ==
== а конкретнее бонус. /                                                 ==
==                                                                       ==
==   BonusBall -- ball that contains bonus.                              ==
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
/*Авторы/Authors: pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь
                       zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь*/
#ifndef BONUSBALL_H
#define BONUSBALL_H
#include "ball.h" //родитель

class BonusBall : public Ball
{
public:
    BonusBall(Field*,double,double,Type,QString&,QGraphicsItem *parent=0);
};

#endif // BONUSBALL_H
