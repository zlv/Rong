#include <QGraphicsView>
#include "windowfield.h"
#include "ball.h"
WindowField::WindowField()
{
    circle_ = new CircleOfDeath();
    ball_ = new Ball(this);
    scene_.addItem(circle_);
    scene_.addItem(ball_);
    circle_->addItems();
    QGraphicsView* view = new QGraphicsView(&scene_);
    resize(800,600);
    view->show();
    setCentralWidget(view);
    startTimer(3);
}

void WindowField::timerEvent(QTimerEvent *)
{
    ball_->moveMe();
    scene_.update();
}
