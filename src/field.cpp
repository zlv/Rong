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
#include "noviceai.h"
#include "score.h"

Field::Field()
{
    ball_ = new Ball(this,2,4);
    circle_ = new CircleOfDeath(this);
    gamer_[0] = new Player(this,0);
    gamer_[1] = new NoviceAI(this,1);
    //нулевой игрок слушает нажатия мыши, а также стрелки, второй игрок --
    gamer_[0]->setMousePressListening(); //это бот, он слушает таймер
    gamer_[1]->setMouseMovedListening();
    gamer_[0]->setKeyrdArrowListening();
    gamer_[1]->setTimerTickdListening();
    score_ = new Score(this); //панель со счётом
}

/*================================
====      Открытые функции      ====
  ================================*/

CircleOfDeath* Field::circle() //круг смерти
{
    return circle_;
}

Gamer* Field::gamer(int i) //итый игрок
{
    return gamer_[i];
}

Ball* Field::ball() //мячик
{
    return ball_;
}

Score * Field::score() //панель счёта
{
    return score_;
}
