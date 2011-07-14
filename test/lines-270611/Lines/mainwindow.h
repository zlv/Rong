#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class Func
{
    double a_,b_;
    bool vertical_;
public:
    Func(double,double, bool v=0);
    Func();
    Func(QLineF);
    bool check(double,double);
    double exec(double);
    bool isVertical();
    double getA()
    {return a_;}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    int _nStick;
    QLineF _stickCoord;
    Func _stickFunc;
    QLineF _line;
    bool _isLine;
    bool _isLineStarted;
    bool _isLaser;
    QLineF laser_;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void cut();
    static bool cmp(double,double);
};

#endif // MAINWINDOW_H
