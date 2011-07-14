/*=========================================================================
==                        windowfield.h                                  ==
==   WindowField -- окно, является оболочкой для поля, включает события  ==
== таймера.                                                              ==
==   View -- виджет рисуемой области, включает в себя события мыши и кла-==
== виатуры/                                                              ==
==                                                                       ==
==   WindowField -- ui for the field.                                    ==
==   View -- widget for mouse and keyboard events.                       ==
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
#ifndef WINDOWFIELD_H
#define WINDOWFIELD_H

#include "field.h" //родитель
#include "settingsdialog.h" //параметры
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>

class FieldData;
class View;

class WindowField : public QMainWindow, public Field
{
    Q_OBJECT

    QGraphicsScene scene_; //сцена для объектов Qt
    SettingsDialog *settingsDialog_; //параметры
    View *view_; //виджет с элементами
    bool pause_; //нажата ли кнопка паузы?
    QAction *newGameAction_; //новая игра
    QAction *settingsAction_; //параметры
    QAction *pauseAction_; //пауза
    QAction *quitAction_; //выход

public:
    WindowField();
    void recreateGamer(int,int); //создать игрока заново
    void update(); //обновить виджет-сцену / update scene widget
    void timerEvent(QTimerEvent *); //основной таймер приложения
private slots:
    void showSettings(); //показать диалог настройки
    void pause(); //установить паузу
private:
    void createMenu(); //создать разные меньюшки
signals:
    //вызывается по таймеру, изменяет данные для бота / called then
    void goBot(FieldData&); //data, needed for bot is changed
};

//обработка событий взаимодействия
class View : public QGraphicsView
{
    Q_OBJECT

    //указатель на окно, в котором мы находимся
    WindowField *field_;
    //переменная нужна, чтобы отслеживать перетаскивание мышью
    bool mousePressed_;

public:
    View(QGraphicsScene*,WindowField*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif // WINDOWFIELD_H
