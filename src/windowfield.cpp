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
#include "magnet.h" //бонусный магнит
#include "circleofdeath.h" //круг смерти
#include "platform.h" //вагонетка
#include "noviceai.h" //новичок-бот
#include "normalai.h" //опытный-бот
#include "score.h" //счёт
#include "player.h" //человеч-игрок
#include "aboutdialog.h" //о программе
#include "settingsdialog.h" //параметры
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QTime> //почтислучайное зерно
#include <QMessageBox>
#include <QSettings>

WindowField::WindowField() : pause_(0)
{
    qsrand(QTime::currentTime().msec());
    //добавить элементы / add all objects to scene
    scene_.addItem(circle_);
    scene_.addItem(magnet_);
    scene_.addItem(score_);
    scene_.addItem(ball_);
    scene_.addItem(bonusBall_);
    for (int i=0; i<2; i++)
        scene_.addItem(extraBalls_[i]);

    /*создать виджет, размещающий объекты и обрабатывающий события мыши и
    клавиатуры / create object for mouse and keyboard tracking*/
    view_ = new View(&scene_,this);
    view_->show();
    setCentralWidget(view_);

    startTimer(25); //таймер

    createMenu(); //создать меню

    readSettings(); //прочитать настройки

    //создать игроков
    if (gamer_[0]==NULL)
    {
        //по умолчанию один из нас человек
        gamer_[0] = new Player(this,0);
        gamer_[0]->setControls(Gamer::MousePressControl);
        gamer_[0]->start();
    }
    if (gamer_[1]==NULL)
    {
        //а второй -- бот
        gamer_[1] = new NormalAI(this,1);
        gamer_[1]->setTimerTickdListening();
        gamer_[1]->start();
        connect(this,SIGNAL(goBot(FieldData&)),
                gamer_[1],SLOT(changeDirection(FieldData&)));
    }

    //настройка
    settingsDialog_ = new SettingsDialog(this,this);
}

//создать игрока заново
void WindowField::recreateGamer(int g, int t)
{
    if (t==-1)
    {
        gamer_[g] = NULL; //игрока не надо создавать / don't need to create
        return;
    }

    disconnect(this,SIGNAL(goBot(FieldData&)));

    Field::recreateGamer(g,t); //создание в другом объекте в общем

    connect(this,SIGNAL(goBot(FieldData&)), //для бота
            (gamer_[g]),SLOT(changeDirection(FieldData&)));
}

void WindowField::update() //обновить виджет-сцену / update scene widget
{
    scene_.update();
}

//основной таймер приложения / first and only one timer of the application
void WindowField::timerEvent(QTimerEvent*)
{
    if (pause_) return; //у нас перерыв, дальше не идём

    if (score_->gameOver()) //проверить окончание игры / check if it is
        gameOver(); //needed to start new game

    //двигать мячи / move balls
    ball_->moveMe();
    if (bonusBall_->showed())
        bonusBall_->moveMe();

    //данные для бота / data for a computer (other thread)
    FieldData data(ball_->point(),circle_->platform(0)->angle(),
                                  circle_->platform(1)->angle(),
                   circle_->limiter(0), circle_->limiter(1),
                   ball_->vx(),ball_->vy(), circle_->radius());

    //при возможности, и если повезёт, создать бонус
    if (canCreateBonus())
        createBonusBall();

    if (currentBonus_!=NoBonus)
        changeBonusState(); //часики для бонуса тикают

    //сообщить боту где мяч, и где его вагонетка / tell bot about us
    emit goBot(data);

    /*сообщить каждому игроку об изменении оставшегося времени курсора и
    каждому боту о том, что нужно уже двигать вагонетку*/
    for (int i=0; i<2; i++)
    {
        if (gamer(i)->type()==Gamer::Human)
            gamer(i)->decCursorTime();
        else
            gamer(i)->timerTickd();
        extraBalls_[i]->moveMe();
    }

    scene_.update();
}

bool WindowField::isPause() //нажата ли пауза / is pause button toggled?
{
    return pause_;
}

void WindowField::readSettings() //читать параметры
{
    QSettings settings("ZKL Inc.","Rong");

    score_->setMaxPoint(settings.value("maxValue_",-1).toInt());
    recreateGamer(0,settings.value("playerType1_",-1).toInt());
    recreateGamer(1,settings.value("playerType2_",-1).toInt());
    setBallsVelocity(settings.value("velocity_",-1).toDouble());
}

void WindowField::writeSettings() //сохранить параметры
{
    QSettings settings("ZKL Inc.","Rong");

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

void WindowField::about() //о программе
{
    AboutDialog dialog;
    dialog.exec();
}

void WindowField::createMenu() //создать разные меньюшки
{
    //новая игра
    newGameAction_  = new QAction(tr("&New game"),this);
    newGameAction_->setShortcut(QKeySequence("Ctrl+N"));
    connect(newGameAction_,SIGNAL(triggered()),this,SLOT(newGame()));
    //настройки игры
    settingsAction_ = new QAction(tr("&Set gamers and ball velocity"),this);
    connect(settingsAction_,SIGNAL(triggered()),this,SLOT(showSettings()));
    settingsAction_->setShortcut(QKeySequence("Esc"));
    //пауза
    pauseAction_ = new QAction(tr("&Pause"),this);
    connect(pauseAction_,SIGNAL(triggered()),this,SLOT(pause()));
    pauseAction_->setShortcut(QKeySequence("P"));
    //выход
    quitAction_     = new QAction(tr("&Quit"),this);
    connect(quitAction_,SIGNAL(triggered()),this,SLOT(close()));
    //большое меню игра
    QMenu *fileMenu = new QMenu(tr("&Game"),this);
    fileMenu->addAction(newGameAction_);
    fileMenu->addAction(settingsAction_);
    fileMenu->addAction(pauseAction_);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction_);
    menuBar()->addMenu(fileMenu);
    //меню справка
    QMenu *helpMenu = new QMenu(tr("&Help"),this);
    QAction *aboutAction = new QAction(tr("&About Rong"),this);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));
    helpMenu->addAction(aboutAction);
    menuBar()->addMenu(helpMenu);
}

//существует ли возможность создать бонус
bool WindowField::canCreateBonus()
{
    /*если нет бонуса, бонусного мяча и ещё если нам повезло /
      set if there are no bonus, no bonus balls and */
    return currentBonus_==NoBonus
        && !bonusBall_->showed()
        && !magnet_->showed()
        && (random(8313)<=3);
}

void WindowField::createBonusBall() //создать мяч бонуса
{
    BonusType bonus = static_cast<BonusType>(random(BonusCount-2)+1);
    if(bonus!=MagnetBonus){
        //обычный бонус
        bonusBall_->setType(bonus);
        bonusBall_->show();
    }
    else
    {
        //магнитобонус
        setBonusTime(MagnetBonus,1);
        magnet_->show();
    }
}

void WindowField::changeBonusState() //проходит время бонуса
{
    if (bonusTime_)
        bonusTime_--;
    if (!bonusTime_) //закончить бонус, когда кончится время
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
        break;
    case MagnetBonus:
        //убрать магнит
        magnet_->hide();
    default:
        break;
    }
}

//показать сообщение об окончании игры
void WindowField::gameOver()
{
    QString sRed  = tr("Red"); //красный
    QString sBlue = tr("Blue"); //синий
    int winner = score_->winner(); //победитель
    QMessageBox::information(this,tr("Game over!"), //выиграл кто-то у нас
                    tr("%1 player wins a game!").arg(winner?sBlue:sRed));
    newGame(); //новая игра
}

//случайное число до max / random number between 0 and max
int WindowField::random(int max)
{
    double qrandNum = qrand();
    return qrandNum/RAND_MAX*(max+1);
}

/*=*=*=*=*=*=*=*=*=*=*
*=*=*=*=*=*=*=*=*=*=*=*
 \\\\\\   View   //////
=*=*=*=*=*=*=*=*=*=*=*=
 =*=*=*=*=*=*=*=*=*=*=*/

View::View(QGraphicsScene *s, WindowField *f) : QGraphicsView(s,f),
    field_(f), mousePressed_(0)
{
    setMouseTracking(1);
}

void View::mousePressEvent(QMouseEvent *e) //кнопка мыши
{
    if (field_->isPause()) return; //если пауза, выходим
    mousePressed_ = 1; //если мыши зажата, следить при движении курсора
    int xinc = size().width()/2; //различия координат виджета и объектов
    int yinc = size().height()/2;
    QPointF point = e->localPos()-QPointF(xinc,yinc); //текущая точка
    for (int i=0; i<2; i++) //сообщить каждому игроку
        field_->gamer(i)->mousePress(point);
}

void View::mouseMoveEvent(QMouseEvent *e) //двиг мыши
{
    if (field_->isPause()) return; //пауза -- выходим / if pause -- quit
    int xinc = size().width()/2; //различия координат виджета и объектов
    int yinc = size().height()/2;
    QPointF point = e->localPos()-QPointF(xinc,yinc); //текущая точка
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
    mousePressed_ = 0; //кнопка отпущена
}

void View::keyPressEvent(QKeyEvent *e) //нажали клавишу
{
    if (field_->isPause()) return;
    keys_.insert(e->key());

    std::set<int>::iterator it;
    for (it=keys_.begin(); it!=keys_.end(); it++) //добавление клавиши
        keyPushed(*it);
}

void View::keyReleaseEvent(QKeyEvent *e) //отпустили
{
    keys_.erase(e->key());
}

void View::keyPushed(int k) //добавление клавиши к списку нажатых
{
    //показывать подсказки постоянно
    if (k==Qt::Key_C || k==Qt::Key_0)
    {
        bool letters = k==Qt::Key_C; //буквы или цифры?
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

