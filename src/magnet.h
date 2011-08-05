/*Авторы/Authors: pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь
                       zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь*/
#ifndef MAGNET_H
#define MAGNET_H

#include <QGraphicsItem> //базовый класс, графический элемент qt
#include <QPointF> //точка
#include <QPixmap>
#include "field.h" //поле

class Magnet : public QGraphicsItem
{
protected:
  double radius_; //радиус мячика
  QPointF point_;
  Field *field_;
  QPixmap pixmap_;
  bool painted_;
  bool showed_;
public:
    Magnet(Field*,QString,QGraphicsItem *parent=0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *,QWidget*);
    QPointF point();
    void show();
    bool showed();
    void hide();
    protected:
    void drawBall();
    void setImage(QString);
};

#endif // MAGNET_H
