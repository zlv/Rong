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
#include "score.h" //это я
#include "circleofdeath.h"
#include <QPainter>
#include <QWidget>

Score::Score(Field *f) : field_(f), maxPoint_(15)
{
    clear();
}

QRectF Score::boundingRect() const //регион отсечения
{
    return QRectF(); //это игнорируется
}

//форма фигуры (для сравнивания)
QPainterPath Score::shape() const
{
    return QPainterPath(); //это тоже
}

void Score::paint(QPainter *p, const QStyleOptionGraphicsItem *,
                     QWidget *w)
{
    bool horizontal = 1; //окно больше по горизонтали?
    if (w->height()>w->width()) horizontal = 0;
    QString sPnt[2];
    for (int i=0; i<2; i++) //получить строковое представление счёта
        sPnt[i] = QString::number(score_[i]);
    int maxSz = sPnt[0].length(); //максимальная длина какой-нибудь строки
    if (maxSz < sPnt[1].length()) //счёта
        maxSz = sPnt[1].length();

    double pointSize; //размер текстовых меток
    if (horizontal) //сравниваемый габарит окна
        pointSize = w->width();
    else
        pointSize = w->height();
    //посчитать размер точки в зависимости от оставшегося свободного места
    pointSize = (pointSize-field_->circle()->radius())/4/maxSz; //на экране
    QFont font; //установить размер шрифта
    font.setPointSizeF(pointSize);
    //координата -- крайняя левая точка экрана
    QPoint coord(-w->width()/2,-w->height()/2+font.pointSize());
    p->setFont(font);
    //нарисовать каждую метку
    p->setPen(Qt::red);
    p->drawText(coord.x(),coord.y(),sPnt[0]);
    p->setPen(Qt::blue);
    p->drawText(coord.x()+font.pointSize()/4*3*maxSz,
                coord.y(),sPnt[1]);
}

//увеличение счёта как ++ / change score (increment by 1), c -- color
void Score::inc(int c)
{
    score_[c]++;
    emit update(); //обновить рисунок
}

//проверить, есть ли победитель / check if game over
bool Score::gameOver()
{
    int winner = -1;
    for (int i=0; i<2; i++)
        if (score_[i]>=maxPoint_) winner = i;
    if (winner!=-1)
    {
        winner_ = winner;
        return 1;
    }
    return 0;
}

//победитель
int Score::winner()
{
    return winner_;
}

//обнулить
void Score::clear()
{
    for (int i=0; i<2; i++)
        score_[i] = 0;
}

//установить максимум очков
void Score::setMaxPoint(int p)
{
    if (p==-1) return;
    maxPoint_ = p;
}

//максимум очков
int Score::maxPoint()
{
    return maxPoint_;
}
