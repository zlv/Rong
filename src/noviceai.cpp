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
#include "noviceai.h" //it's me
#include "circleofdeath.h" //круг смерти
#include "platform.h" //вагонетка
#include "constants.h" //пи
#include <math.h>

const double NoviceAI::novicedang=PI/36;

NoviceAI::NoviceAI(Field* f, int p) : Gamer(f,p,AI),direction(0){}

void NoviceAI::timerTickd(int)
{
    if (listening_&timerTickdListening && direction)
        field_->circle()->platform(platform_)->changeAngle(dang*direction);
}

//изменить переменные
void NoviceAI::changeDirection(QPointF& p, double angle)
{
    mutex.lock();
    ballPoint_ = p;
    angle_ = angle;
    mutex.unlock();
}

//работа мозга бота во время которой он решает куда ему двигаться дальше
void NoviceAI::run()
{
    while (1)
    {
        mutex.lock();
        double tempAngle; //вычислить угол
        tempAngle = acos(ballPoint_.x()/sqrt(ballPoint_.y()*ballPoint_.y()
                                         +ballPoint_.x()*ballPoint_.x()));
        tempAngle = field_->circle()->limiter(platform_)+(platform_?
                                    PI-tempAngle:tempAngle);
        //разница текущего угла и угла, нужного чтобы отбить мячик
        double delta = tempAngle-angle_;
        if (fabs(delta)>novicedang)
            direction = delta>0?1:-1;
        else
            direction = 0;
        mutex.unlock();
    }
}
