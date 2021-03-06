/*=========================================================================
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
/*Авторы/Authors: pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь
                       zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь*/
#include "bonusball.h"
#include <QPainter>
#include <QWidget>
#include <math.h>
#include <QDebug>
#include "gamer.h"
#include "constants.h"
#include "circleofdeath.h"
#include "platform.h"
#include "score.h"

BonusBall::BonusBall(Field* f, double v, Type t,
                     QString sImg, QGraphicsItem *parent)
    : Ball(f,v,t,sImg,parent)
{
    showed_=0;
}

void BonusBall::moveMe() //движение
{
    if (!painted_ || !showed_) return;
    bool crossBorder = !collidesWithItem(field_->circle(),Qt::ContainsItemShape);//если перестаёт пересекаться с кругом смерти
    bool crossPlatform[2];//если пересекается с одной из вагонеток
    //bool crossBall = collidesWithItem(field_->ball());
    for (int i=0; i<2; i++)//2 вагонетки
    {
        crossPlatform[i] = collidesWithItem(field_->circle()->platform(i));//пересекается ли с iтой вагонеткой
    }
    if (crossBorder)//если каснулся вагонеток, или перестал касаться круга смерти
    {
        double arc;//угол прямой к оси X от которой отражается шарик
        //угол касательной к окружности, в точке, где шарик перестаёт пересекаться с кругом смерти
        arc=acos(point_.y()/(sqrt(point_.x()*point_.x()+point_.y()*point_.y())));
        //угол к оси X прямой, от которой отражается шарик не больше 90
        arc=(arc>PI/2?PI-arc:arc);
        //изменение вектора движения шарика
        mirror(arc);
    }
    if (crossPlatform[0]||crossPlatform[1])
    {
        int gamer=-1;
        if(crossPlatform[0])
            gamer=1;
        else
            gamer=0;

        field_->setBonusTime(bonusType_,gamer);
        hide();
        return;
    }

    //изменение координат шарика в соответствии с вектором движения
    point_=QPointF(point_.x()+vx_,point_.y()+vy_);
}

void BonusBall::setType(BonusType bt)
{
    bonusType_ = bt;
    setImage(findFileName(bt));
}

QString BonusBall::findFileName(BonusType bt) //имя файла по имени бонуса
{
    switch (bt)
    {
    case PlusBallsBonus:
        return ":/images/vkp0.png";
    case IncPlatformSizeBonus:
        return ":/images/vkp1.png";
    case ChangeDirectionBonus:
        return ":/images/vkp3.png";
    default:
        return "";
    }
}

void BonusBall::show()
{
    point_ = QPoint(0,0);
    showed_ = 1;
    setRandomAngleVelocity(sqrt(vx_*vx_+vy_*vy_));
}
