/*=========================================================================
==                             noviceai.h                                ==
==   NoviceAI -- начинающий бот. /                                       ==
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
#include <QMutex>

class NoviceAI : public Gamer
{
    Q_OBJECT

    QMutex mutex; //мьютекс для взаимодействия мозга бота и игры
    QPointF ballPoint_; //координаты мяча
    volatile double angle_; //угол нашей вагонетки / angle of platform
    volatile int direction; //1 -- право, 0 -- центр, -1 -- лево
public:
    //угол, который должен быть до мяча, чтобы компьютер
    static const double novicedang; //сделал шаг

    NoviceAI(Field*,int);
    void timerTickd(int);
public slots:
    //изменить переменные
    void changeDirection(QPointF&,double);
protected:
    //работа мозга бота во время которой он решает
    void run(); //куда ему двигаться дальше
};

#endif // NOVICEAI_H
