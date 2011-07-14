#include "mainwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include <stdlib.h>
#include <math.h>
#include <iostream>

//предопределённые координаты
QLineF stickCoords[] = {QLineF(QPointF(100,300),QPointF(500,300)),
                        QLineF(QPointF(100,500),QPointF(500,100)),
                        QLineF(QPointF(300,500),QPointF(300,100)),
                        QLineF(QPointF(500,500),QPointF(100,100))};
//найти начало отражения и само отражение / find reflection
void MainWindow::cut()
{
    Func lineFunc(_line);
    double inc=0.0005;
    if (_line.x1()>_line.x2()) inc = -inc;
    bool horizontal = _stickFunc.getA()==0;
    for (double i=_line.x1(); !cmp(i,_line.x2()); i+=inc)
    {
        QPointF point;
        if (_stickFunc.isVertical())
        {
            double y = lineFunc.exec(i);
            if (!cmp(_stickCoord.x1(),i) ||
               (_stickCoord.y1()<_stickCoord.y2() &&
               (y<_stickCoord.y1() || y>_stickCoord.y2())) ||
               (_stickCoord.y1()>_stickCoord.y2() &&
               (y>_stickCoord.y1() || y<_stickCoord.y2())))
                continue;
            point = QPoint(_line.x1(),lineFunc.exec(i)*2-_line.y1());
        }
        else
        {
            double y = lineFunc.exec(i);
            if (!cmp(y,_stickFunc.exec(i)) ||
               (_stickCoord.x1()<_stickCoord.x2() &&
               (i<_stickCoord.x1() || i>_stickCoord.x2())) ||
               (_stickCoord.x1()>_stickCoord.x2() &&
               (i>_stickCoord.x1() || i<_stickCoord.x2())))
                continue;
            if (horizontal)
            {
                point = QPoint(i*2-_line.x1(),_line.y1());
            }
            else
            {
                double fallAngle;
                double y3 = _stickFunc.exec(_line.x1());
                double y2 = lineFunc.exec(i);
                double ay = y3-_line.y1();
                double ap = ay*ay;
                double bx = i -_line.x1();
                double by = y2-_line.y1();
                double bp = bx*bx+by*by;
                double cy = y3-y2; //cx=bx
                double cp = bx*bx+cy*cy;
                double b = sqrt(bp);
                double fallAngleCos = (bp+cp-ap)/(2*b*sqrt(cp));
                fallAngle = acos(fallAngleCos);
                double stickAngle=atan(_stickFunc.getA());
                double angle;
                bool down=_line.y1()>_stickFunc.exec(_line.x1());
                if (bx>=0 && !down)
                {
                    angle = fallAngle-stickAngle;
                    point = QPoint(i+b*cos(angle),y2-b*sin(angle));
                }
                else if (bx<0 && !down)
                {
                    angle = fallAngle+stickAngle;
                    point = QPointF(i-b*cos(angle),y2-b*sin(angle));
                }else if (bx>=0 && down)
                {
                    angle = -fallAngle-stickAngle;
                    point = QPoint(i+b*cos(angle),y2-b*sin(angle));
                }
                else if (bx<0 && down)
                {
                    angle = -fallAngle+stickAngle;
                    point = QPointF(i-b*cos(angle),y2-b*sin(angle));
                }

                /*std::cerr << "firstangle: " << fallAngle/3.14159265*180
                          << " sticangle: " << stickAngle/3.14159265*180
                          << " finiangle: " << angle/3.14159265*180
                          << std::endl;*/
            }
        }
        _isLaser = 1;
        laser_ = QLineF(QPointF(i,lineFunc.exec(i)),point);
        return;
    }
    _isLaser = 0;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _nStick(0), _stickCoord(stickCoords[0]),
      _stickFunc(_stickCoord), _isLine(0), _isLineStarted(0), _isLaser(0)
{
    setGeometry(5,5,600,600); //установить размеры окна
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(_stickCoord);
    if (_isLine)
        painter.drawLine(_line);
    if (_isLaser)
        painter.drawLine(laser_);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    bool flag=1;
    bool fla =1;
    int inc=1;
    if (e->modifiers()==Qt::CTRL) inc+=10;
    if (e->key()==Qt::Key_W)
        _stickCoord.setP2(_stickCoord.p2()+QPointF(0,-inc));
    else if (e->key()==Qt::Key_S)
        _stickCoord.setP2(_stickCoord.p2()+QPointF(0, inc));
    else if (e->key()==Qt::Key_A)
        _stickCoord.setP2(_stickCoord.p2()+QPointF(-inc,0));
    else if (e->key()==Qt::Key_D)
        _stickCoord.setP2(_stickCoord.p2()+QPointF( inc,0));
    else fla  = 0;
    if (fla)
    {
        _stickFunc = Func(_stickCoord);
        if (_isLine)
            cut();
        emit update();
    }
    if (e->key()==Qt::Key_Up)
        _line.setP2(_line.p2()+QPointF(0,-inc));
    else if (e->key()==Qt::Key_Down)
        _line.setP2(_line.p2()+QPointF(0, inc));
    else if (e->key()==Qt::Key_Left)
        _line.setP2(_line.p2()+QPointF(-inc,0));
    else if (e->key()==Qt::Key_Right)
        _line.setP2(_line.p2()+QPointF( inc,0));
    else flag = 0;
    if (flag)
    {
        cut();
        emit update();
    }
    if (e->key()!=Qt::Key_R) return;
    _nStick = (_nStick+1)%4;
    _stickCoord = stickCoords[_nStick];
    _stickFunc = Func(_stickCoord);
    if (_isLine)
        cut();
    emit update();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (_isLine)
    {
        _isLine = 0;
        _isLineStarted = 0;
        _isLaser = 0;
    }
    else if (_isLineStarted)
    {
        _line.setP2(e->pos());
        _isLine = 1;
        cut();
    }
    else
    {
        _line.setP1(e->pos());
        _isLineStarted = 1;
    }
    emit update();
}

//сравнение вещественных / compare
bool MainWindow::cmp(double a, double b)
{
    const double eps = 1e-6;
    if (abs(a-b)<eps) return 1;
    return 0;
}

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
    if (MainWindow::cmp(a_*x+b_,y)) return 1;
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
