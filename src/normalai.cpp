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
#include "normalai.h" //эт'о йа
#include "circleofdeath.h" //круг смерти
#include "platform.h" //вагонетка
#include "constants.h" //пи
#include <math.h>

//угол, который должен быть до мяча, чтобы компьютер сделал шаг
const double NormalAI::normaldang=PI/72; //(чем меньше, тем лучше)

NormalAI::NormalAI(Field* f, int p) : Gamer(f,p,Normal),direction_(0){}

//перемещать платформу по событию таймера / move element on timer event
void NormalAI::timerTickd()
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
void NormalAI::changeDirection(FieldData& d)
{
    initialized_ = 1;
    ballPoint_ = d.ballPoint;
    angle_ = d.platformAngle[platform_];
    limiter_ = d.platformLimiter[platform_];
    vx_ = d.vx;
    if (directionSwaped_)
    {
        vx_ = -vx_;
        ballPoint_.setX(-ballPoint_.x());
    }
    vy_ = d.vy;
    circleRadius_ = d.circleRadius;
}

//работа мозга бота во время которой он решает куда ему двигаться дальше /
void NormalAI::run() //where to run next?
{
    while (!initialized_); //ждать начала / waiting for begin
    while (1)
    {
        if (stopped_) break; //стоп!
        /*выбрать случайный угол, если мяч летит не на нас /
          if ball flies not in our direction*/
        if (((vy_>=0 &&  platform_) || (vy_<0  && !platform_)))
        {
            if (!angleSpec_)
            {
                angleSpec_ = 1;
                tempAngle_ = limiter_+rand()*PI/RAND_MAX;
            }
        }
        else //нужно подставиться туда, куда прилетит мяч
        {
            QPointF point(ballPoint_);
            angleSpec_ = 0;
            //радиус в квадрате / radius power 2
            double radiusp2 = circleRadius_*circleRadius_;
            /*найти точку, куда мяч прилетит /
              find point where ball should be*/
            while (point.x()*point.x()+point.y()*point.y()<radiusp2)
                point += QPointF(vx_,vy_);
            //найти угол
            tempAngle_ = acos(point.x()/sqrt(point.x()*point.x()
                                            +point.y()*point.y()));
            //изменить угол в зависимости от номера вагонетки
            tempAngle_ = limiter_+(platform_?PI-tempAngle_:tempAngle_);
        }
        //разница текущего угла и угла, нужного чтобы отбить мячик
        double delta = tempAngle_-angle_;
        if (fabs(delta)>normaldang)
        {
            direction_ = delta>0?1:-1;
        }
        else
        {
            direction_ = 0;
        }
    }
}
