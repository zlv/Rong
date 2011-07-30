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
#include "normalai.h"
#include "score.h"
#include "player.h"
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QTime>
#include <QMessageBox>
#include <QSettings>

WindowField::WindowField() : pause_(0)
{
    qsrand(QTime::currentTime().msec());
    //добавить элементы
    scene_.addItem(circle_);
    scene_.addItem(ball_);
    scene_.addItem(bonusBall_);
    scene_.addItem(score_);
    /*создать виджет, размещающий объекты и обрабатывающий события мыши и
    клавиатуры*/
    view_ = new View(&scene_,this);
    view_->show();
    setCentralWidget(view_);
    startTimer(25); //таймер
    for (int i=0; i<2; i++)
        scene_.addItem(extraBalls_[i]);
    createMenu();
    readSettings();
    if (gamer_[0]==NULL)
    {
        //создать для начала бота и человека
        gamer_[0] = new Player(this,0);
        gamer_[0]->setControls(Gamer::MousePressControl);
        gamer_[0]->start();
    }
    if (gamer_[1]==NULL)
    {
        gamer_[1] = new NormalAI(this,1);
        //управление
        gamer_[1]->setTimerTickdListening();
        gamer_[1]->start();
        connect(this,SIGNAL(goBot(FieldData&)), //для бота
                (gamer_[1]),SLOT(changeDirection(FieldData&)));
    }
    settingsDialog_ = new SettingsDialog(this,this);
}

//создать игрока заново
void WindowField::recreateGamer(int g, int t)
{
    //игрока не надо создавать
    if (t==-1)
    {
        gamer_[g] = NULL;
        return;
    }
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
    if (score_->gameOver()) //проверить окончание игры
        gameOver();
    ball_->moveMe(); //двигать мяч
    if (bonusBall_->showed())
        bonusBall_->moveMe();
    //данные для бота
    FieldData data(ball_->point(),circle_->platform(0)->angle(),
                                  circle_->platform(1)->angle(),
                   circle_->limiter(0), circle_->limiter(1),
                   ball_->vx(),ball_->vy(), circle_->radius());

    if (canCreateBonus())
        createBonusBall();

    if (currentBonus_!=NoBonus)
        changeBonusState();

    //сообщить боту где мяч, и где его ваго-
    emit goBot(data); //нетка
    for (int i=0; i<2; i++) //сообщить каждому игроку об изменении времени
    {
        if (gamer(i)->type()==Gamer::Human)
            gamer(i)->decCursorTime();
        else
            gamer(i)->timerTickd();
        extraBalls_[i]->moveMe();
    }
    scene_.update();
}

bool WindowField::isPause() //нажата ли пауза
{
    return pause_;
}

void WindowField::readSettings() //читать параметры
{
    QSettings settings("BDL Inc.","Rong");

    score_->setMaxPoint(settings.value("maxValue_",-1).toInt());
    recreateGamer(0,settings.value("playerType1_",-1).toInt());
    recreateGamer(1,settings.value("playerType2_",-1).toInt());
    setBallsVelocity(settings.value("velocity_",-1).toDouble());
}

void WindowField::writeSettings() //сохранить параметры
{
    QSettings settings("BDL Inc.","Rong");

    settings.setValue("maxValue_",score_->maxPoint());
    settings.setValue("playerType1_",gamer(0)->type());
    settings.setValue("playerType2_",gamer(1)->type());
    settings.setValue("velocity_",ball_->velocity());
}

void WindowField::closeEvent(QCloseEvent *e) //закрыть
{
    writeSettings();
    e->accept();
}

void WindowField::newGame() //обнулить счёт, начать новую игру
{
    score_->clear(); //обнулить счёт
    ball_->renew(); //переместить мяч
    bonusBall_->hide(); //скрыть бонус
    if (bonusTime_)
        endBonus(currentBonus_); //завершить бонус
    bonusTime_ = 0;
    currentBonus_ = NoBonus;
    swappedDirGamer_ = 0;
}

void WindowField::showSettings() //показать диалог настройки
{
    emit settingsDialog_->show();
}

void WindowField::pause() //установить/убрать паузу
{
    pause_ = !pause_;
}

void WindowField::createMenu() //создать разные меньюшки
{
    newGameAction_  = new QAction(tr("&New game"),this);
    newGameAction_->setShortcut(QKeySequence("Ctrl+N"));
    connect(newGameAction_,SIGNAL(triggered()),this,SLOT(newGame()));
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

//существует ли возможность создать бонус
bool WindowField::canCreateBonus()
{
    return currentBonus_==NoBonus
        && !bonusBall_->showed()
        && (random(8313)<=3);
}

void WindowField::createBonusBall() //создать мяч бонуса
{
    BonusType bonus = static_cast<BonusType>(random(BonusCount-2)+1);
    bonusBall_->setType(bonus);
    bonusBall_->show();
}

void WindowField::changeBonusState() //проходит время бонуса
{
    if (bonusTime_)
        bonusTime_--;
    if (!bonusTime_)
    {
        endBonus(currentBonus_);
        currentBonus_ = NoBonus;
    }
}

void WindowField::endBonus(BonusType t) //завершить бонус
{
    switch (t)
    {
    case PlusBallsBonus:
        rmf2Balls(); //убрать 2 мяча
        break;
    case IncPlatformSizeBonus:
        //вернуть размеры платформ
        circle_->platform(0)->returnPlatformSize();
        circle_->platform(1)->returnPlatformSize();
        break;
    case ChangeDirectionBonus:
        //вернуть направления
        gamer_[swappedDirGamer_]->deswapDirection();
    default:
        break;
    }
}

//показать сообщение об окончании игры
void WindowField::gameOver()
{
    QString sRed  = tr("Red");
    QString sBlue = tr("Blue");
    int winner = score_->winner(); //победитель
    QMessageBox::information(this,tr("Game over!"),
                    tr("%1 player wins a game!").arg(winner?sBlue:sRed));
    newGame(); //новая игра
}

//случайное число до max / random number between 0 and max
int WindowField::random(int max)
{
    double qrandNum = qrand();
    return qrandNum/RAND_MAX*(max+1);
}

View::View(QGraphicsScene *s, WindowField *f) : QGraphicsView(s,f),
    field_(f), mousePressed_(0)
{
    setMouseTracking(1);
}

void View::mousePressEvent(QMouseEvent *e) //кнопка мыши
{
    if (field_->isPause()) return;
    mousePressed_ = 1; //если мыши зажата, следить при движении курсора
    int xinc = size().width()/2; //различия координат виджета и объектов
    int yinc = size().height()/2;
    QPointF point = e->posF()-QPointF(xinc,yinc); //текущая точка
    for (int i=0; i<2; i++) //сообщить каждому игроку
        field_->gamer(i)->mousePress(point);
}

void View::mouseMoveEvent(QMouseEvent *e)
{
    if (field_->isPause()) return;
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
    if (field_->isPause()) return;
    keys_.insert(e->key());

    std::set<int>::iterator it;
    for (it=keys_.begin(); it!=keys_.end(); it++)
        keyPushed(*it);
}

void View::keyReleaseEvent(QKeyEvent *e)
{
    keys_.erase(e->key());
}

void View::keyPushed(int k)
{
    //показывать подсказки постоянно
    if (k==Qt::Key_C || k==Qt::Key_0)
    {
        bool letters = k==Qt::Key_C;
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
        field_->gamer(i)->keyPressed(k);
}

