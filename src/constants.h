/*=========================================================================
==                           constants.h                                 ==
==   Некоторые общедоступные константы необходимые всем обитателям игры. ==
==                                                                       ==
==   А также функции, которых нет в математической библиотеке /          ==
==                                                                       ==
==   There is a PI, and math functions.                                  ==
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
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "math.h"

const double PI = 3.14159265359;

//сравнение двух вещественных
bool equal(double a, double b);

//точность сравнения
const double EPS = 1e-9;

#endif // CONSTANTS_H
