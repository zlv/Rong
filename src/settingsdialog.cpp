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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "ball.h"
#include "constants.h"
#include "windowfield.h"

SettingsDialog::SettingsDialog(Field *f, QWidget *parent) :
    QDialog(parent), field_(f), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    velocity_ = f->ball()->velocity(); //запомнить скорость
    //поместить значение скорости в виджет
    emit ui->startVelLineEdit->setText(QString::number(velocity_));
    //получить другую информацию
    for (int i=0; i<2; i++)
    {
        type_[i] = field_->gamer(i)->type();
        controls_[i] = field_->gamer(i)->controls();
    }
    //установка других виджетов
    ui->player01ComboBox->setCurrentIndex(type_[0]);
    ui->player02ComboBox->setCurrentIndex(type_[1]);
    ui->player01ControlsComboBox->setCurrentIndex(controls_[0]);
    ui->player02ControlsComboBox->setCurrentIndex(controls_[1]);
    connect(ui->player01ComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(indexFor01Changed(int)));
    connect(ui->player02ComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(indexFor02Changed(int)));
    if (type_[0]==Gamer::Human)
        ui->player01ControlsComboBox->setEnabled(1);
    if (type_[1]==Gamer::Human)
        ui->player02ControlsComboBox->setEnabled(1);
    //кнопки
    connect(ui->okgoodButton,SIGNAL(clicked()),this,SLOT(okgood()));
    connect(ui->apconfButton,SIGNAL(clicked()),this,SLOT(apconf()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(hide()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::okgood() //Готово
{
    apconf(); //применить
    emit hide(); //закрыть
}

//Применить / apply configuration
void SettingsDialog::apconf()
{
    //указанная пользователем скорость
    double newVelocity = ui->startVelLineEdit->text().toDouble();
    if (!equal(velocity_,newVelocity))
    { //если она не равна предыдущей изменить
        field_->ball()->setVelocity(newVelocity);
    }
    //установка игроков и управления тоже
    Gamer::Type newType;
    Gamer::Controls newControls;
    newType = static_cast<Gamer::Type>(ui->player01ComboBox->currentIndex());
    if (type_[0]!= newType)
    {
        type_[0] = newType;
        static_cast<WindowField*>(field_)->recreateGamer(0,newType);
    }
    if (type_[0]==Gamer::Human)
    {
        newControls = static_cast<Gamer::Controls>(ui->player01ControlsComboBox->currentIndex());
        if (controls_[0]!= newControls)
        {
            controls_[0] = newControls;
            field_->gamer(0)->setControls(newControls);
        }
    }
    newType = static_cast<Gamer::Type>(ui->player02ComboBox->currentIndex());
    if (type_[1]!= newType)
    {
        type_[1] = newType;
        static_cast<WindowField*>(field_)->recreateGamer(1,newType);
    }
    if (type_[1]==Gamer::Human)
    {
        newControls = static_cast<Gamer::Controls>(ui->player02ControlsComboBox->currentIndex());
        if (controls_[1]!= newControls)
        {
            controls_[1] = newControls;
            field_->gamer(1)->setControls(newControls);
        }
    }
}

//изменён индекс выбора игрока / player index changed
void SettingsDialog::indexFor01Changed(int i)
{
    bool enabled = 0;
    if (i==Gamer::Human)
         enabled = 1;
    ui->player01ControlsComboBox->setEnabled(enabled);
}

//изменён индекс выбора игрока / player index changed
void SettingsDialog::indexFor02Changed(int i)
{
    bool enabled = 0;
    if (i==Gamer::Human)
         enabled = 1;
    ui->player02ControlsComboBox->setEnabled(enabled);
}
