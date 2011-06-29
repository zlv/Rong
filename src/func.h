#ifndef FUNC_H
#define FUNC_H

#include <QLineF>

/*Функция прямой / Line Function*/
class Func
{
    double a_,b_; /*a -- тангенс угла наклона касательной, b -- прирост*/
    bool vertical_; //вертикальная функция не имеет a
public:
    Func(double,double, bool v=0); //v -- vertical
    Func();
    Func(QLineF);
    bool check(double,double); //проверить, принадлежит ли точка функции
    double exec(double); //узнать значение в зависимости от аргумента
    bool isVertical();
    double getA();

    bool cmp(double,double); //сравнение вещественных / compare
};

#endif // FUNC_H
