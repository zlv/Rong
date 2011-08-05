/*=========================================================================
==                       settingsdialog.h                                ==
==   SettingsDialog -- диалог для изменения параметров. Скорость, коли-  ==
==  чество очков для победы, опции игроков./                             ==
==                                                                       ==
==   SettingsDialog -- dialog for changing options.                      ==
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
#ifndef FORM_H
#define FORM_H

#include <QDialog>
#include "gamer.h"

namespace Ui {
    class SettingsDialog;
}

class Field;

class SettingsDialog : public QDialog
{
    Q_OBJECT

    Field *field_; //указатель на поле
    double velocity_; //установленная скорость мяча
    int maxPoints_; //максимум очков
    Gamer::Type type_[2]; //установленные типы игроков / types of gamers
    Gamer::Controls controls_[2]; //типы управления / control types
    Ui::SettingsDialog *ui; //виджеты в диалоге

public:
    explicit SettingsDialog(Field*,QWidget *parent = 0);
    ~SettingsDialog();
private slots:
    void okgood(); //Готово
    void apconf(); //Применить / apply configuration
    //изменён индекс выбора игрока / player index changed
    void indexFor01Changed(int);
    //изменён индекс выбора игрока / player index changed
    void indexFor02Changed(int);
};

#endif // FORM_H
