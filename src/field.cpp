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
#include "field.h" //этот класс
#include "ball.h"
#include "circleofdeath.h"
#include "player.h"

Field::Field()
{
    ball_ = new Ball(this);
    circle_ = new CircleOfDeath(this);
    for (int i=0; i<2; i++) //пока что оба игроки
        gamer_[i] = new Player(this,i);
    /*пока что: нулевой игрок слушает нажатия мыши, первый слушает движения
    мышки, оба игрока слушают стрелки*/
    gamer_[0]->setMousePressListening();
    gamer_[1]->setMouseMovedListening();
    gamer_[0]->setKeyrdArrowListening();
    gamer_[1]->setKeyrdArrowListening();
}

/*================================
====      Открытые функции      ====
  ================================*/

CircleOfDeath* Field::circle() //круг смерти
{
    return circle_;
}

Gamer * Field::gamer(int i) //итый игрок
{
    return gamer_[i];
}
