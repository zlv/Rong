/*=========================================================================
==                        windowfield.h                                  ==
==   WindowField -- окно, является оболочкой для поля.                   ==
=========================================================================*/
/*Авторы/Authors: zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь
             pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь*/
#ifndef WINDOWFIELD_H
#define WINDOWFIELD_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPoint>
#include "field.h" //родитель

class WindowField : public QMainWindow, public Field
{
    Q_OBJECT

    QGraphicsScene scene_; //сцена для объектов Qt
public:
    WindowField();
    void timerEvent(QTimerEvent *); //основной таймер приложения
};

#endif // WINDOWFIELD_H
