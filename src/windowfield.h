/*=========================================================================
==                        windowfield.h                                  ==
==   WindowField -- окно, является оболочкой для поля. /                 ==
==                                                                       ==
==   WindowField -- ui for the field                                     ==
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

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPoint>
#include "field.h" //родитель

class WindowField : public QMainWindow, public Field
{
    Q_OBJECT

    QGraphicsScene scene_; //сцена для объектов Qt
public:
    WindowField();
    void update(); //обновить виджет-сцену / update scene widget
    void timerEvent(QTimerEvent *); //основной таймер приложения
};

//обработка событий взаимодействия
class View : public QGraphicsView
{
    Q_OBJECT

    WindowField *field_;
    bool mousePressed_;
public:
    View(QGraphicsScene*,WindowField*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif // WINDOWFIELD_H
