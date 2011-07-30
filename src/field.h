/*=========================================================================
==                           field.h                                     ==
==   Field -- игровое поле, содержащее в себе всё необходимое для игры.  ==
==   Описание:                                                           ==
==   Структура содержащая разные объекты, которые могут взаимодействовать==
== друг с другом через это поле. /                                       ==
==                                                                       ==
==   Field -- field, that should contain all gaming objects.             ==
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
#include <vector>

class Ball; //мячик, объявлен в другом месте / declaration is not here
class CircleOfDeath; //круг смерти тоже
class Gamer; //и они тоже
class Bonus;
class BonusBall;
class Score;
//тип бонуса
enum BonusType{NoBonus=0,PlusBallsBonus,IncPlatformSizeBonus,
               ChangeDirectionBonus,BonusCount};
class Field
{
protected:
    Ball *ball_; //мячик
    Ball *extraBalls_[2]; //дополнительные мячи
    CircleOfDeath *circle_; //круг
    Gamer *gamer_[2]; //игроки
    Bonus *bonus_; //текущий бонусы
    BonusBall *bonusBall_; //мячик, несущий бонус
    Score *score_; //табличка со счётом
    int bonusTime_; //длительность бонуса
    BonusType currentBonus_; //тип действующего бонуса
    //игрок, на которого действует бонус / gamer with bonus
    int swappedDirGamer_;
public:
    //выводить ненужную информацию, а также вспомогательные линии /
    static const bool debug=0; //print text and red lines

    Field();
    //пересоздать игрока / create gamer again
    virtual void recreateGamer(int,int);
    //функции доступа
    Ball* ball();
    CircleOfDeath* circle();
    Gamer* gamer(int);
    Score* score();
    //установить параметры бонусов / set bonus settings
    void setBonusTime(BonusType,int);
    //включить дополнительные мячи / activate extra balls
    void add2Balls();
    //отлючить дополнительные мячи / detivate extra balls
    void rmf2Balls();
    //установить новую скорость мячей
    void setBallsVelocity(double);
};

#endif // FIELD_H
