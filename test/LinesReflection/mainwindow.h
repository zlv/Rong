/*************************************************************************
                              mainwindow.h
  mainwindow -- основное окно тестовой программы LinesReflection.
Описание:
  В окне нарисована одна единственная платформа на которую падает луч (луч
рисуете вы). Изменять наклон платформы можно с помощью клавиши R (Rotate --
русское К), а также с помощью клавиш w,a,s,d можно изменять расположение
второй точки платформы. Ctrl -- ускорить перемещение Луч можно рисовать
мышкой (первое нажатие -- первая точка, второе нажатие -- закончить линию,
третие нажатие -- убрать линию). Перемещать нарисованный луч можно с по-
мощью стелок. Ctrl тоже ускоряет движение.
  Программа создана исключительно для целей тестирования отражения лучей
для программы Rong (Или 2Круг смерти", клон Понга). /

  mainwindow -- window for testing purposes for program LinesReflection.
Description:
  There is stick, which catch ray (it should be drawing by mouse). You can
rotate stick with R, or change position of one of its points by w,a,s,d
(ctrl is for quick changing position). You can place ray by two clicks,
third click will erase ray. You can control your ray by arrows.
  This program is created only for testing program Rong (Pong clone).
*************************************************************************/
/*Авторы/Authors: zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь
             pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

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
};

//само окно
class MainWindow : public QMainWindow
{
    Q_OBJECT

    int _nStick; //номер предопределённой платформы / number of stick
    QLineF _stickCoord; //координаты платформы
    Func _stickFunc; //функция к платформе / function for stick
    QLineF _line; //луч / ray
    bool _isLine; //нарисована ли линия
    bool _isLineStarted; //начала ли линия рисоваться
    bool _isLaser; //отражается ли линия / is it reflects
    QLineF laser_; //координаты отражения / reflection

    void cut(); //найти начало отражения и само отражение / find reflection
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    static bool cmp(double,double); //сравнение вещественных / compare
};

#endif // MAINWINDOW_H
