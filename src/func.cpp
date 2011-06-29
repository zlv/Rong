#include "func.h"
#include <stdlib.h>
#include <math.h>

Func::Func(double a, double b, bool v) : a_(a), b_(b), vertical_(v){}

Func::Func() : a_(0), b_(0), vertical_(0){}

Func::Func(QLineF l) : vertical_(0)
{
    if ((l.x2()-l.x1())!=0)
        a_ = (l.y2()-l.y1())/
             (l.x2()-l.x1());
    else
        vertical_ = 1;
    b_ = vertical_?l.x1():(l.y1()-l.x1()*a_);
}

//проверить, принадлежит ли точка функции / срусл if point in function
bool Func::check(double x, double y)
{
    if (cmp(a_*x+b_,y)) return 1;
    return 0;
}

//узнать значение в зависимости от аргумента
double Func::exec(double x)
{
    return x*a_+b_;
}

bool Func::isVertical()
{
    return vertical_;
}

double Func::getA()
{
    return a_;
}

//сравнение вещественных / compare
bool Func::cmp(double a, double b)
{
    const double eps = 1e-6;
    if (abs(a-b)<eps) return 1;
    return 0;
}
