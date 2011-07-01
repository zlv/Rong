/*=========================================================================
==                           field.h                                     ==
==   Field -- игровое поле, содержащее в себе всё необходимое для игры.  ==
==   Описание:                                                           ==
==   Структура содержащая разные объекты, которые могут взаимодействовать==
== друг с другом через это поле. /                                       ==
==                                                                       ==
==   Field -- field, that should contain all gaming objects              ==
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
#ifndef FIELD_H
#define FIELD_H

class Ball; //мячик, объявлен в другом месте / declaration is not here
class CircleOfDeath; //круг смерти тоже
class Gamer; //и он тоже
class Field
{
protected:
    Ball *ball_; //мячик
    CircleOfDeath *circle_; //круг
    Gamer *gamer_[2]; //игроки
public:

    Field();
    //функции доступа
    CircleOfDeath* circle();
    Gamer* gamer(int);
};

#endif // FIELD_H
