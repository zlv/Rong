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
#include "player.h"
#include "circleofdeath.h"
#include "platform.h"
#include "constants.h"

const double Player::dang=PI/72; //изменение угла / delta angle

Player::Player(Field* f, int p) : Gamer(f,p,Human){}

/*================================================
===       Унаследованные от Игрока функции       ===
  ================================================*/
//переопределение функций Игрока / redefinition of Gamer's functions

void Player::mouseMoved(QPointF& p) //движение мыши
{
    if (listening_&mouseMovedListening)
        emit field_->circle()->platform(platform_)->changeAngle(p);
}

void Player::mousePress(QPointF& p) //нажатие на кнопку мыши
{
    if (listening_&mousePressListening)
        emit field_->circle()->platform(platform_)->changeAngle(p);
}

void Player::keyPressed(int k) //нажатие на стрелку
{
    if (listening_&keyrdArrowListening)
    {
        Platform *platform = field_->circle()->platform(platform_);
        switch (k)
        {
        case Qt::Key_Up:
        case Qt::Key_Left:
            //движение происходит влево, а не по часовой стрелке; поэтому в
            emit platform->changeAngle((platform_?-1:1)*dang);//зависимости
            break;//от номера вагонетки ставится знак перед изменением угла
        case Qt::Key_Down:
        case Qt::Key_Right:
            emit platform->changeAngle((platform_?1:-1)*dang);
            break;
        }

    }
}
