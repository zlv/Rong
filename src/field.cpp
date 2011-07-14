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

Field::Field()
{
    ball_ = new Ball(this,16);
    QString filename="../images/vkp3.png";
    bonusBall_ = new BonusBall(this,2,Ball::BonusBall,filename);
    circle_ = new CircleOfDeath(this);
    //создать для начала бота и человека
    gamer_[0] = new Player(this,0);
    gamer_[1] = new NormalAI(this,1);
    gamer_[0]->setControls(Gamer::MousePressControl);
    gamer_[1]->setTimerTickdListening();
    score_ = new Score(this); //панель со счётом
    for (int i=0; i<2; i++) //включить бота (может это и не бот, но нам
        gamer_[i]->start();  //совершенно пофиг)
}

/*================================
====      Открытые функции      ====
  ================================*/

//пересоздать игрока
void Field::recreateGamer(int g, int t)
{
    //убить существующего
    gamer_[g]->stop();
    while (gamer_[g]->type()!=Gamer::Human && !gamer_[g]->isFinished());
    delete gamer_[g];
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
