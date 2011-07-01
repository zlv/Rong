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
#include "windowfield.h" //this class
#include <QGraphicsView>
#include <QMouseEvent>
#include "ball.h"
#include "circleofdeath.h"
#include "gamer.h"

WindowField::WindowField()
{
    //добавить элементы
    scene_.addItem(circle_);
    scene_.addItem(ball_);
    /*создать виджет, размещающий объекты и обрабатывающий события мыши и
    клавиатуры*/
    View* view = new View(&scene_,this);
    view->show();
    setCentralWidget(view);
    setMouseTracking(1);
    startTimer(3);
}

void WindowField::update() //обновить виджет-сцену / update scene widget
{
    scene_.update();
}

//основной таймер приложения
void WindowField::timerEvent(QTimerEvent *)
{
    ball_->moveMe(); //двигать мяч
    scene_.update();
}

View::View(QGraphicsScene *s, WindowField *f) : QGraphicsView(s,f),
    field_(f), mousePressed_(0)
{
    setMouseTracking(1); //следи за мышью
}

void View::mousePressEvent(QMouseEvent *e) //кнопка мыши
{
    mousePressed_ = 1; //если мыши зажата, следить при движении курсора
    int xinc = size().width()/2; //различия координат виджета и объектов
    int yinc = size().height()/2;
    QPointF point = e->posF()-QPointF(xinc,yinc); //текущая точка
    for (int i=0; i<2; i++) //сообщить каждому игроку
        field_->gamer(i)->mousePress(point);
    field_->update();
}

void View::mouseMoveEvent(QMouseEvent *e)
{
    int xinc = size().width()/2; //различия координат виджета и объектов
    int yinc = size().height()/2;
    QPointF point = e->posF()-QPointF(xinc,yinc); //текущая точка
    for (int i=0; i<2; i++) //сообщить каждому игроку
    {
        field_->gamer(i)->mouseMoved(point);
        if (!mousePressed_) continue;
        field_->gamer(i)->mousePress(point);
    }
    field_->update();
}

void View::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed_ = 0;
}

void View::keyPressEvent(QKeyEvent *e)
{
    for (int i=0; i<2; i++) //сообщить каждому игроку
        field_->gamer(i)->keyPressed(e->key());
    field_->update();
}
