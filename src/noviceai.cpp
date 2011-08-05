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
#include "field.h" //поле, на котором размещён весь мир
#include <math.h>

//угол, который должен быть до мяча, чтобы компьютер сделал шаг
const double NoviceAI::novicedang=PI/36; //(чем меньше, тем лучше)

NoviceAI::NoviceAI(Field* f, int p) : Gamer(f,p,Novice),direction_(0){}

//перемещать платформу по событию таймера / move element on timer event
void NoviceAI::timerTickd()
{
    if (listening_&timerTickdListening && direction_)
    {
        //увеличить или уменьшить угол
        field_->circle()->platform(platform_)->changeAngle(dang*direction_);
        //проверить на заход на другую платформу
        checkRedo();
        //больше не двигаться
        direction_ = 0;
    }
}

//изменить переменные
void NoviceAI::changeDirection(FieldData& d)
{
    initialized_ = 1;
    ballPoint_ = d.ballPoint;
    if (directionSwaped_)
        ballPoint_.setX(-ballPoint_.x());
    angle_ = d.platformAngle[platform_];
    limiter_ = d.platformLimiter[platform_];
}

//работа мозга бота во время которой он решает куда ему двигаться дальше
void NoviceAI::run()
{
    while (!initialized_); //ждать начала / waiting for begin
    while (1)
    {
        if (stopped_) break; //стоп!
        double tempAngle; //вычислить угол
        tempAngle = acos(ballPoint_.x()/sqrt(ballPoint_.y()*ballPoint_.y()
                                            +ballPoint_.x()*ballPoint_.x()));
        tempAngle = limiter_+(platform_?PI-tempAngle:tempAngle);
        //разница текущего угла и угла, нужного чтобы отбить мячик
        double delta = tempAngle-angle_;
        if (fabs(delta)>novicedang)
            direction_ = delta>0?1:-1;
        else
            direction_ = 0;
    }
}
