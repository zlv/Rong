/*=========================================================================
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
#include "player.h" //это мы
#include "circleofdeath.h"
#include "platform.h"
#include "constants.h"

Player::Player(Field* f, int p) : Gamer(f,p,Human),
    showCursor_(0){}

/*================================================
===       Унаследованные от Игрока функции       ===
  ================================================*/
//переопределение функций Игрока / redefinition of Gamer's functions

void Player::mouseMoved(QPointF& p) //движение мыши
{
    if (listening_&mouseMovedListening)
    {
        emit field_->circle()->platform(platform_)->changeAngle(p);
        checkRedo();
    }
}

void Player::mousePress(QPointF& p) //нажатие на кнопку мыши
{
    if (listening_&mousePressListening)
    {
        emit field_->circle()->platform(platform_)->changeAngle(p);
        checkRedo();
    }
}

void Player::keyPressed(int k) //нажатие на стрелку
{
    //подслушивать стрелки
    if (listening_&keyrdArrowListening)
    {
        Platform *platform = field_->circle()->platform(platform_);
        bool key = 1;
        if (listening_&keyrdSpeclListening)
        {
            switch (k)
            {
            case Qt::Key_Up:
                cursor_.setY(cursor_.y()-dcoord);
                break;
            case Qt::Key_Left:
                cursor_.setX(cursor_.x()-dcoord);
                break;
            case Qt::Key_Down:
                cursor_.setY(cursor_.y()+dcoord);
                break;
            case Qt::Key_Right:
                cursor_.setX(cursor_.x()+dcoord);
                break;
            default:
                key = 0;
            }
            if (key)
            {
                //показать курсор (или подсказку), если была нажата
                showCursor(); //какая-нибудь из наших клавиш
                checkRedo();
            }
        }
        else if (listening_&keyrdMovegListening)
        {
            switch (k)
            {
            case Qt::Key_Up:
            case Qt::Key_Left:
                //движение происходит влево, а не по часовой стрелке; поэтому в
                emit platform->changeAngle((platform_?-1:1)*dang);//зависимости
                checkRedo();
                break;//от номера вагонетки ставится знак перед изменением угла
            case Qt::Key_Down:
            case Qt::Key_Right:
                emit platform->changeAngle((platform_?1:-1)*dang);
                checkRedo();
                break;
            default:
                key = 0;
            }
        }
        if (key)
        {
            //показать курсор (или подсказку), если была нажата
            showCursor(); //какая-нибудь из наших клавиш
            emit platform->changeAngle(cursor_);
        }
    }
    //альтернативные стрелки-буквы
    if (listening_&keyrdWasdkListening)
    {
        Platform *platform = field_->circle()->platform(platform_);
        bool key = 1;
        if (listening_&keyrdSpeclListening)
        {
            switch (k)
            {
            case Qt::Key_W:
                cursor_.setY(cursor_.y()-dcoord);
                break;
            case Qt::Key_A:
                cursor_.setX(cursor_.x()-dcoord);
                break;
            case Qt::Key_S:
                cursor_.setY(cursor_.y()+dcoord);
                break;
            case Qt::Key_D:
                cursor_.setX(cursor_.x()+dcoord);
                break;
            default:
                key = 0;
            }
            if (key)
            {
                showCursor();
                checkRedo();
            }
        }
        else if (listening_&keyrdMovegListening)
        {
            switch (k)
            {
            case Qt::Key_W:
            case Qt::Key_A:
                //движение происходит влево, а не по часовой стрелке; поэтому в
                emit platform->changeAngle((platform_?-1:1)*dang);//зависимости
                checkRedo();
                break;//от номера вагонетки ставится знак перед изменением угла
            case Qt::Key_S:
            case Qt::Key_D:
                emit platform->changeAngle((platform_?1:-1)*dang);
                checkRedo();
                break;
            default:
                key = 0;
            }
        }
        if (key)
        {
            showCursor();
            emit platform->changeAngle(cursor_);
        }
    }
    if (listening_&keyrdWasdkListening)
    {
        Platform *platform = field_->circle()->platform(platform_);
        bool key = 1;
        if (listening_&keyrdSpeclListening)
        {
            switch (k)
            {
            case Qt::Key_W:
                cursor_.setY(cursor_.y()-dcoord);
                break;
            case Qt::Key_A:
                cursor_.setX(cursor_.x()-dcoord);
                break;
            case Qt::Key_S:
                cursor_.setY(cursor_.y()+dcoord);
                break;
            case Qt::Key_D:
                cursor_.setX(cursor_.x()+dcoord);
                break;
            default:
                key = 0;
            }
            if (key)
            {
                showCursor();
                checkRedo();
            }
        }
        else if (listening_&keyrdMovegListening)
        {
            switch (k)
            {
            case Qt::Key_W:
            case Qt::Key_A:
                //движение происходит влево, а не по часовой стрелке; поэтому в
                emit platform->changeAngle((platform_?-1:1)*dang);//зависимости
                checkRedo();
                break;//от номера вагонетки ставится знак перед изменением угла
            case Qt::Key_S:
            case Qt::Key_D:
                emit platform->changeAngle((platform_?1:-1)*dang);
                checkRedo();
                break;
            default:
                key = 0;
            }
        }
        if (key)
        {
            showCursor();
            emit platform->changeAngle(cursor_);
        }
    }
    //цифры для быстрого перехода к области круга
    if (listening_&keyrdNumbsListening)
    {
        Platform *platform = field_->circle()->platform(platform_);
        switch (k)
        {
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            int num = k-Qt::Key_1;
            double angle = PI+(platform_?+1:-1)*PI*(num*2+1)/18;
            emit platform->setAngle(angle);
            checkRedo();
            showCursor();
            break;
        }
    }
    //буквы для быстрого перехода к области круга
    if (listening_&keyrdAnumsListening)
    {
        Platform *platform = field_->circle()->platform(platform_);
        int num;
        bool key = 1;
        switch (k)
        {
        case Qt::Key_V:
            num = 0;
            break;
        case Qt::Key_B:
            num = 1;
            break;
        case Qt::Key_N:
            num = 2;
            break;
        case Qt::Key_G:
            num = 3;
            break;
        case Qt::Key_H:
            num = 4;
            break;
        case Qt::Key_J:
            num = 5;
            break;
        case Qt::Key_Y:
            num = 6;
            break;
        case Qt::Key_U:
            num = 7;
            break;
        case Qt::Key_I:
            num = 8;
            break;
        default:
            key = 0;
        }
        if (key)
        {
            double angle = PI+(platform_?+1:-1)*PI*(num*2+1)/18;
            emit platform->setAngle(angle);
            checkRedo();
            showCursor();
        }
    }
}

void Player::decCursorTime() //срабатывает по событию таймера
{
    if (!showCursor_) return;
    if (--cursorTime_==0)
        hideCursor();
}

//установить курсор на точку c / c -- cursor point
void Player::setCursor(QPointF& c)
{
    cursor_ = c;
    field_->circle()->update();
}

//доступ к курсору
QPointF Player::getCursor()
{
    return cursor_;
}

//показывается ли курсор
bool Player::isShowCursor()
{
    return showCursor_;
}

//показать курсор
void Player::showCursor()
{
    showCursor_ = 1;
    if (cursorTime_!=-1)
        cursorTime_ = cursorTimeStd;
    field_->circle()->update();
}

//показать курсор без ограничения показа
void Player::showCursorPermanent()
{
    showCursor_ = 1;
    cursorTime_ = -1;
    field_->circle()->update();
}

//скрыть
void Player::hideCursor()
{
    showCursor_ = 0;
    field_->circle()->update();
}

bool Player::isCrossShape() //надо ли показывать перекрестие
{
    if (controls_==KeyrdNumbsAndArrowsSpec
     || controls_==KeyrdLetrsOnlySpec)
        return 1;
    return 0;
}

bool Player::isNumbsShape() //надо ли показывать посказку с цифрами
{
    if (controls_==KeyrdNumbsAndArrows
     || controls_==KeyrdNumbsAndArrowsSpec)
        return 1;
    return 0;
}

bool Player::isLetrsShape() //надо ли показывать посказку с буквами
{
    if (controls_==KeyrdLetrsOnly
     || controls_==KeyrdLetrsOnlySpec)
        return 1;
    return 0;
}
