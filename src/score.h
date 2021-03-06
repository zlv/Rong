/*=========================================================================
==                            score.h                                    ==
==   Score -- область в углу [может и не в углу], в которую пишется счёт ==
== разноцветными цифрами. /                                              ==
==                                                                       ==
==   Score -- game score panel in a corner.                              ==
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
#ifndef SCORE_H
#define SCORE_H
#include <QGraphicsItem> //графический элемент

class Field;

class Score : public QGraphicsItem
{
    Field *field_; //поле, на котором мы
    int score_[2]; //очки
    int maxPoint_; //максимум очков (если дошли, вышли)
    int winner_; //номер победителя

public:
    Score(Field*);
    //унаследованные функции
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *,QWidget*);
    //увеличение счёта как ++ / change score (increment by 1)
    void inc(int);
    bool gameOver(); //проверить, есть ли победитель / check if game over
    int winner(); //победитель
    void clear(); //обнулить
    void setMaxPoint(int); //установить максимум очков
    int maxPoint(); //максимум очков
};

#endif // SCORE_H
