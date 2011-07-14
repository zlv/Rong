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
#include "ball.h" //мячик
#include "bonusball.h" // бонусный мячик
#include "circleofdeath.h" //круг смерти
#include "platform.h"
#include "noviceai.h"
#include "score.h"
#include "player.h"
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>

WindowField::WindowField() : pause_(0)
{
    //добавить элементы
    scene_.addItem(circle_);
    scene_.addItem(ball_);
    //scene_.addItem(bonusBall_);
    scene_.addItem(score_);
    /*создать виджет, размещающий объекты и обрабатывающий события мыши и
    клавиатуры*/
    view_ = new View(&scene_,this);
    view_->show();
    setCentralWidget(view_);
    startTimer(20); //таймер
    for (int i=0; i<2; i++)
        connect(this,SIGNAL(goBot(FieldData&)), //для бота
                (gamer_[i]),SLOT(changeDirection(FieldData&)));
    createMenu();
    settingsDialog_ = new SettingsDialog(this,this);
}

void WindowField::recreateGamer(int g, int t)
{
    disconnect(this,SIGNAL(goBot(FieldData&)));
    Field::recreateGamer(g,t);

    connect(this,SIGNAL(goBot(FieldData&)), //для бота
            (gamer_[g]),SLOT(changeDirection(FieldData&)));
}

void WindowField::update() //обновить виджет-сцену / update scene widget
{
    scene_.update();
}

//основной таймер приложения
void WindowField::timerEvent(QTimerEvent*)
{
    if (pause_) return;
    ball_->moveMe(); //двигать мяч
    bonusBall_->moveMe();
    FieldData data(ball_->point(),circle_->platform(0)->angle(),
                                  circle_->platform(1)->angle(),
                   circle_->limiter(0), circle_->limiter(1),
                   ball_->vx(),ball_->vy(), circle_->radius());

    //сообщить боту где мяч, и где его ваго-
    emit goBot(data); //нетка
    for (int i=0; i<2; i++) //сообщить каждому игроку об изменении времени
    {
        if (gamer(i)->type()==Gamer::Human)
            gamer(i)->decCursorTime();
        else
            gamer(i)->timerTickd();
    }
    scene_.update();
}

void WindowField::showSettings()
{
    emit settingsDialog_->show();
}

void WindowField::pause()
{
    pause_ = !pause_;
}

void WindowField::createMenu()
{
    newGameAction_  = new QAction(tr("&New game"),this);
    settingsAction_ = new QAction(tr("&Set gamers and ball velocity"),this);
    connect(settingsAction_,SIGNAL(triggered()),this,SLOT(showSettings()));
    settingsAction_->setShortcut(QKeySequence("Esc"));
    pauseAction_ = new QAction(tr("&Pause"),this);
    connect(pauseAction_,SIGNAL(triggered()),this,SLOT(pause()));
    pauseAction_->setShortcut(QKeySequence("P"));
    quitAction_     = new QAction(tr("&Quit"),this);
    connect(quitAction_,SIGNAL(triggered()),this,SLOT(close()));
    QMenu *fileMenu = new QMenu(tr("&File"),this);
    fileMenu->addAction(newGameAction_);
    fileMenu->addAction(settingsAction_);
    fileMenu->addAction(pauseAction_);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction_);
    menuBar()->addMenu(fileMenu);
}

View::View(QGraphicsScene *s, WindowField *f) : QGraphicsView(s,f),
    field_(f), mousePressed_(0)
{
    setMouseTracking(1);
}

void View::mousePressEvent(QMouseEvent *e) //кнопка мыши
{
    mousePressed_ = 1; //если мыши зажата, следить при движении курсора
    int xinc = size().width()/2; //различия координат виджета и объектов
    int yinc = size().height()/2;
    QPointF point = e->posF()-QPointF(xinc,yinc); //текущая точка
    for (int i=0; i<2; i++) //сообщить каждому игроку
        field_->gamer(i)->mousePress(point);
}

void View::mouseMoveEvent(QMouseEvent *e)
{
    int xinc = size().width()/2; //различия координат виджета и объектов
    int yinc = size().height()/2;
    QPointF point = e->posF()-QPointF(xinc,yinc); //текущая точка
    for (int i=0; i<2; i++) //сообщить каждому игроку
    {
        field_->gamer(i)->mouseMoved(point);
        if (!mousePressed_) continue; //кнопка мыши не зажата
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
    //показывать подсказки постоянно
    if (e->key()==Qt::Key_C || e->key()==Qt::Key_0)
    {
        bool letters = e->key()==Qt::Key_C;
        for (int i=0; i<2; i++)
        {
            Player *player = static_cast<Player*>(field_->gamer(i));
            if ((player->isLetrsShape() &&  letters) ||
                (player->isNumbsShape() && !letters))
            {
                if (!player->isShowCursor())
                    player->showCursorPermanent();
                else
                    player->hideCursor();
            }
        }
        return;
    }
    for (int i=0; i<2; i++) //сообщить каждому игроку
        field_->gamer(i)->keyPressed(e->key());
}
