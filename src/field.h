/*=========================================================================
==                           field.h                                     ==
==   Field -- игровое поле, содержащее в себе всё необходимое для игры.  ==
==   Описание:                                                           ==
==   Структура содержащая разные объекты, которые могут взаимодействовать==
== друг с другом через это поле. /                                       ==
==                                                                       ==
==   Field -- field, that should contain all gaming objects              ==
=========================================================================*/
/*Авторы/Authors: zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь
             pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь*/
#ifndef FIELD_H
#define FIELD_H
#include "circleofdeath.h" //круг смерти

class Ball; //мячик, объявлен в другом месте
class Field
{
protected:
    Ball *ball_; //мячик
    CircleOfDeath *circle_; //круг
public:
    Field();
    CircleOfDeath* circle();
};

#endif // FIELD_H
