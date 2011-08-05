/*=========================================================================
==                           gamer.h                                     ==
==   Gamer -- играющее существо (бот или человек), способно управлять по-==
== лем (точнее одной из вагонеток на нём). Имеет тип и больше ничего /   ==
==                                                                       ==
==   Gamer -- class for control platforms (it can be bot or human).      ==
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
#ifndef GAMER_H
#define GAMER_H
#include <QThread> //любой объект этого вида может думать в отдельном потоке
#include <QPointF> //точка

struct FieldData;
class Field;

class Gamer : public QThread
{
    Q_OBJECT

public:
    //тип игрока
    enum Type{Novice, Normal, Human};
    //как можно управлять людьми
    enum Controls{MousePressControl,MouseMoveControl,
                  KeyrdNumbsAndArrows,KeyrdNumbsAndArrowsSpec,
                  KeyrdLetrsOnly,KeyrdLetrsOnlySpec};

protected:
    static const double dang; //изменение угла / delta angle
    static const int mouseMovedListening=0x01; //слушать мышь
    static const int mousePressListening=0x02; //слушать нажатия мыши
    static const int keyrdArrowListening=0x04; //слушать стрелки
    static const int keyrdWasdkListening=0x08; //слушать альтер стрелки
    static const int keyrdNumbsListening=0x10; //слушать быстрые цифры
    static const int keyrdAnumsListening=0x20; //слушать быстрые буквы
    static const int keyrdSpeclListening=0x40; //фрушать клава-курсор
    static const int keyrdMovegListening=0x80; //слушать клава-направление
    static const int timerTickdListening=0x100; //слушать часы
    int platform_; //номер вагонетки, связанной с нами / number of platform
    Type type_; //кто ты? ангел или бес
    int listening_; //что мы слушаем?
    Field *field_; //связывающее всех поле
    Controls controls_; //тип управления
    volatile bool stopped_; //остановлен ли поток
    bool directionSwaped_; //было ли управление изменено бонусом?

public:
    Gamer(Field*,int,Type);
    void stop(); //остановить поток
    void setControls(Controls); //указать управление
    virtual void mouseMoved(QPointF&); //движение мыши
    virtual void mousePress(QPointF&); //нажатие на кнопку мыши
    virtual void keyPressed(int); //нажатие на стрелку
    virtual void timerTickd(); //прошло время
    virtual void decCursorTime(); //уменишить время подсказки
    void setMouseMovedListening(); //слушать ли движение мыши
    void setMousePressListening(); //слушать ли нажатия кнопок мыши
    void setKeyrdArrowListening(); //слушать ли стрелки
    void setTimerTickdListening(); //слушать ли время
    Type type(); //вернуть тип
    Controls controls(); //какое управление
    void swapDirection(); //переменить направление
    void deswapDirection(); //поменить направление
public slots:
    //изменить переменные
    virtual void changeDirection(FieldData&);
protected:
    //проверить, так ли изменён угол / correct angle
    void checkRedo();
};

struct FieldData //данные о поле
{
    QPointF ballPoint; //точка, где мяч / where is ball?
    double platformAngle[2]; //где находятся вагонетки / where is platforms?
    double platformLimiter[2]; //до куда могут дойти
    double vx; //скорость / speed projection
    double vy;
    double circleRadius; //размер круга смерти (он нужен)

    FieldData(QPointF,double,double,double,double);
    FieldData(QPointF,double,double,double,double,double,double,double);
};


#endif // GAMER_H
