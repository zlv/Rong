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
#include "circleofdeath.h" //этот класс
#include "constants.h" //Пи
#include "platform.h" //вагонетки
#include "ball.h" //мяч нужен для определения контуров
#include "player.h" //из него мы узнаем о том, нужно ли показывать подсказки для игрока
#include <QPainter> //рисование
#include <QWidget>  //размеры окна
#include <math.h>

const QString CircleOfDeath::letters[9] =
    {QObject::tr("V"),QObject::tr("B"),QObject::tr("N"),
     QObject::tr("G"),QObject::tr("H"),QObject::tr("J"),
     QObject::tr("Y"),QObject::tr("U"),QObject::tr("I")};

const double CircleOfDeath::limitEmptySpace = PI/36; //ширина ограничителей

/*эталонный радиус окружности смерти; радиус круга смерти сравнивается
с этой константой, и платформа увеличивается или уменьшается /
  radius of the circle of death, that affects on the platform size*/
const double CircleOfDeath::neededRadius = 250;

CircleOfDeath::CircleOfDeath(Field *f, QGraphicsItem *parent) :
    QGraphicsItem(parent), radius_(0), field_(f)
{
    for (int i=0; i<2; i++) //установка ограничений и платформ
        limiter_[i] = PI*i; //ограничетели через каждую Пи
    for (int i=0; i<2; i++) //создание двух вагонеток
        platform_[i] = new Platform(i,this);
    alpha_[0] = (limiter_[0]+limitEmptySpace)/2/PI*5760;//градусная мера
    alpha_[1] = (limiter_[1]-limitEmptySpace)/2/PI*5760;//1/16 градуса; пол-
    alpha_[2] = (limiter_[1]+limitEmptySpace)/2/PI*5760;//ный угол оборота
    alpha_[3] = (1+(limiter_[0]-limitEmptySpace)/2/PI)*5760;//равен 360*16=5760
}

/*================================
===    Унаследованные функции    ===
  ================================*/

QRectF CircleOfDeath::boundingRect() const //регион отсечения
{
    qreal penWidth = 1;
    return QRectF(-radius_-penWidth/2,-radius_-penWidth/2,
                  radius_*2+penWidth, radius_*2+penWidth);
}

//форма фигуры (для сравнивания)
QPainterPath CircleOfDeath::shape() const
{
    QPainterPath path;
    path.addEllipse(-radius_,-radius_,radius_*2,radius_*2);
    return path;
}

//рисование
void CircleOfDeath::paint(QPainter *p, const QStyleOptionGraphicsItem *,
                          QWidget *w)
{
    QSize size = w->size(); //размер окна / size of the window
    radius_ = size.width()<size.height()? //минимальный габарит
              size.width():size.height();
    radius_=radius_/2.2; //радиус равен половине окна + круг смерти должен
                     //быть меньше, чем окно
    QColor colors[] = {Qt::blue, Qt::red}; //цвета полуокружностей

    p->setRenderHint(QPainter::Antialiasing,true); //включить сглаживание
    QPen pen;
    pen.setWidth(3); //установить ширину линии

    for (int i=0; i<2; i++) //рисование каждой полуокружности
    {
        pen.setColor(colors[i]);
        p->setPen(pen);
        p->drawArc(-radius_,-radius_,radius_*2,radius_*2,alpha_[i*2],
                   alpha_[i*2+1]-alpha_[i*2]);
    }

    //рисование подсказок
    for (int playerNum=0; playerNum<2; playerNum++)
    {
        //нужно рисовать, только если это игрок
        if (field_->gamer(playerNum)->type()!=Gamer::Human)
            continue;
        Player *player = static_cast<Player*>(field_->gamer(playerNum));
        if (player->isShowCursor()) //если курсор нужно показывать
        {
            if (player->isCrossShape()) //показать перекрестье
            {
                //выбрать цвет, такой же, как у игрока
                if (playerNum==0)
                    p->setPen(Qt::red);
                else
                    p->setPen(Qt::blue);
                const double cursorSize = 8;
                p->drawLine(player->getCursor()-QPoint(cursorSize/2,0),
                            player->getCursor()+QPoint(cursorSize/2,0));
                p->drawLine(player->getCursor()-QPoint(0,cursorSize/2),
                            player->getCursor()+QPoint(0,cursorSize/2));
            }
            //рисовать подсказки с названиями клавиш возле круга
            p->setPen(Qt::gray);
            //цифры
            if (player->isNumbsShape())
            {
                for (int i=0; i<9; i++)
                {
                    QPoint coord;
                    //угол от 0 до 180
                    double angle = PI+(playerNum?+1:-1)*(i*2+1)*PI/18;
                    //разместить возле круга
                    coord.setX(1.05*radius_*cos(angle));
                    coord.setY(1.05*radius_*sin(angle)+p->font().pointSize()/2);
                    p->drawText(coord,QString::number(i+1));
                }
            }
            //буквы
            if (player->isLetrsShape())
            {
                for (int i=0; i<9; i++)
                {
                    QPoint coord;
                    double angle = PI+(playerNum?+1:-1)*(i*2+1)*PI/18;
                    coord.setX(1.05*radius_*cos(angle));
                    coord.setY(1.05*radius_*sin(angle)+p->font().pointSize()/2);
                    p->drawText(coord,letters[i]);
                }
            }
        }
    }
    //нарисовать ненужные игроку картинки
    if (Field::debug)
    {
        pen.setWidth(1);
        p->setPen(pen);
        p->drawRect(platform(0)->boundingRect());
        p->drawRect(platform(1)->boundingRect());
        p->drawRect(boundingRect());
        p->drawRect(field_->ball()->boundingRect());
        p->drawPath(platform(0)->shape());
        p->drawPath(platform(1)->shape());
        p->drawPath(shape());
        p->drawPath(field_->ball()->shape());
    }
}

/*================================
====      Открытые функции      ====
  ================================*/

//вернуть номер вагонетки по одной точке
int CircleOfDeath::getColor(QPointF& p)
{
    double angle = atan(p.x()/p.y())+PI/2;
    if (p.y()>0) angle = PI-angle;
    if (p.y()<0) angle = 2*PI-angle;
    if (angle<limiter_[1]-limitEmptySpace
     && angle>limiter_[0]+limitEmptySpace)
        return 0;
    else if (angle<limiter_[0]-limitEmptySpace+2*PI
          && angle>limiter_[1]+limitEmptySpace)
        return 1;
    return 2;
}

double CircleOfDeath::radius() //радиус круга
{
    return radius_;
}

double CircleOfDeath::limiter(int i) const //его итый ограничетель
{
    return limiter_[i];
}

Platform * CircleOfDeath::platform(int i) //его итая вагонетка
{
    return platform_[i];
}
