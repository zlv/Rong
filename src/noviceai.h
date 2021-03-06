/*=========================================================================
==                             noviceai.h                                ==
==   NoviceAI -- начинающий бот. Тупит, бегая за мячом буквально. Всегда ==
== бегает, достаёт даже./                                                ==
==                                                                       ==
==   NoviceAI -- bot-beginner.                                           ==
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
#ifndef NOVICEAI_H
#define NOVICEAI_H
#include "gamer.h"

class NoviceAI : public Gamer
{
    Q_OBJECT

    //нужно ли потоку выполнять какие-нибудь
    volatile bool initialized_; //действия
    QPointF ballPoint_; //координаты мяча
    volatile double angle_; //угол нашей вагонетки / angle of platform
    volatile double limiter_; //её минимальный угол
    //1 -- право/right, 0 -- центр, -1 -- лево/left
    volatile int direction_;
public:
    //угол, который должен быть до мяча, чтобы компьютер
    static const double novicedang; //сделал шаг

    NoviceAI(Field*,int);
    //перемещать платформу по событию таймера / move element on timer event
    void timerTickd();
public slots:
    void changeDirection(FieldData&); //изменить переменные
protected:
    //работа мозга бота во время которой он решает
    void run(); //куда ему двигаться дальше
};

#endif // NOVICEAI_H
