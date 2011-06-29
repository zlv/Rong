#include "circleofdeath.h"
#include <QPainter>
#include <QWidget>
#include <QGraphicsScene>
#include "constants.h"

const double CircleOfDeath::limitEmptySpace = PI/72;

CircleOfDeath::CircleOfDeath(QGraphicsItem *parent) :
    QGraphicsItem(parent), radius_(0)
{
    for (int i=0; i<2; i++) //установка ограничений и платформ
    {
        limiter_[i] = PI*i;
        platform_[i] = new Platform(i,this);
    }
}

/*================================
===    Унаследованные функции    ===
  ================================*/

QRectF CircleOfDeath::boundingRect() const //регион отсечения
{
    qreal penWidth = 1;
    return QRectF(-radius_-penWidth/2,-radius_-penWidth/2,
                   radius_*2+penWidth, radius_*2+penWidth);
}

//рисование
void CircleOfDeath::paint(QPainter *p, const QStyleOptionGraphicsItem *,
                          QWidget *w)
{
    QSize size = w->size(); //размер окна / size of the window
    radius_ = size.width()<size.height()?
              size.width():size.height();
    radius_=radius_/2.2; //радиус равен половине окна + круг смерти должен
    int alpha[4];//углы //быть меньше, чем окно
    alpha[0] = (limiter_[0]+limitEmptySpace)/2/PI*5760;//градусная мера
    alpha[1] = (limiter_[1]-limitEmptySpace)/2/PI*5760;//1/16 градуса, пол-
    alpha[2] = (limiter_[1]+limitEmptySpace)/2/PI*5760;//ный угол оборота
    alpha[3] = (limiter_[0]-limitEmptySpace)/2/PI*5760;//равен 360*16=5760
    QColor colors[] = {Qt::blue, Qt::red}; //цвета полуокружностей

    p->setRenderHint(QPainter::Antialiasing,true); //сглаживание
    QPen pen;
    pen.setWidth(3); //установить длину линии

    for (int i=0; i<2; i++)
    {
        pen.setColor(colors[i]);
        p->setPen(pen);
        p->drawArc(-radius_,-radius_,radius_*2,radius_*2,alpha[i*2],
                   (alpha[i*2+1]<0?(5760+alpha[i*2+1]):alpha[i*2+1])-
                    alpha[i*2]);
    }
}

//форма фигуры (для сравнивания)
QPainterPath CircleOfDeath::shape() const
{
    QPainterPath path;
    path.addEllipse(-radius_,-radius_,radius_*2,radius_*2);
    return path;
}

/*================================
====      Открытая функция      ====
  ================================*/

void CircleOfDeath::addItems()
{
    for (int i=0; i<2; i++) //довавить 2-е вагонетки
        scene()->addItem(platform_[i]);
}
