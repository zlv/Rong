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
#include "bonusball.h" // бонусный мячик
#include "circleofdeath.h"
#include "player.h"
#include "noviceai.h"
#include "normalai.h"
#include "score.h"
#include "platform.h" //вагонетки

Field::Field() : bonusTime_(0), currentBonus_(NoBonus),
    swappedDirGamer_(0)
{
    //мячик со скоростью побольше
    ball_ = new Ball(this,18);
    //бонус с небольшой скоростью
    bonusBall_ = new BonusBall(this,2,Ball::BonusBall);
    //круг смерти
    circle_ = new CircleOfDeath(this);
    score_ = new Score(this); //панель со счётом
    for (int i=0; i<2; i++)
    {
        gamer_[i] = NULL;
        //включить мячик
        extraBalls_[i] = new Ball(this,ball_->velocity());
        //и скрыть его
        extraBalls_[i]->hide();
    }
}

/*================================
====      Открытые функции      ====
  ================================*/

//пересоздать игрока / create gamer again
void Field::recreateGamer(int g, int t)
{
    if (gamer_[g]!=NULL)
    {
        //убить существующего
        gamer_[g]->stop();
        //подождать завершения работы игрока
        while (gamer_[g]->type()!=Gamer::Human && !gamer_[g]->isFinished());
        delete gamer_[g]; //убить его
    }
    //создать нового, смотря на тип / create new gamer with type t
    Gamer::Type type = static_cast<Gamer::Type>(t);
    switch (type)
    {
        case Gamer::Novice:
            gamer_[g] = new NoviceAI(this,g);
            gamer_[g]->setTimerTickdListening();
            break;
        case Gamer::Normal:
            gamer_[g] = new NormalAI(this,g);
            gamer_[g]->setTimerTickdListening();
            break;
        case Gamer::Human:
            gamer_[g] = new Player(this,g);
            gamer_[g]->setMousePressListening();
            break;
    }
    //запустить его поток
    emit gamer_[g]->start();
}

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

//установить параметры бонусов / set bonus settings
void Field::setBonusTime(BonusType cb, int swdg)
{
    if (currentBonus_==NoBonus) //бонус пока не установлен
    {
        switch (cb)
        {
            case PlusBallsBonus:
                //добавление мячей
                add2Balls();
                bonusTime_ = 1000;
                break;
            case IncPlatformSizeBonus:
                //увеличение вагонетки
                circle_->platform(!swdg)->incSize();
                //уменьшение вагонетки
                circle_->platform( swdg)->decSize();
                bonusTime_ = 1200;
                break;
            case ChangeDirectionBonus:
                gamer_[swdg]->swapDirection();
                bonusTime_ = 200;
            default:
                break;
        }
    }
    currentBonus_ = cb;
    swappedDirGamer_ = swdg;
}

//включить дополнительные мячи / activate extra balls
void Field::add2Balls()
{
    for (int i=0; i<2; i++)
        extraBalls_[i]->renew();
}

//отлючить дополнительные мячи / detivate extra balls
void Field::rmf2Balls()
{
    for (int i=0; i<2; i++)
        extraBalls_[i]->hide();
}

//установить новую скорость мячей / nv -- new velocity
void Field::setBallsVelocity(double nv)
{
    ball_->setVelocity(nv);
    for (int i=0; i<2; i++)
    {
        extraBalls_[i]->setVelocity(nv);
    }
}
