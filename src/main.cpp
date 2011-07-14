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
/*Авторы/Authors: pavertomato(Егор Лежнин) <pavertomato@gmail.com>, 2011 -- Томск->Сибирь
                       zlv(Евгений Лежнин) <z_lezhnin@mail2000.ru>, 2011 -- Томск->Сибирь*/
#include "windowfield.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    //загрузка языка / load language
    QTranslator translator;
    if (QLocale::system().name().indexOf("ru")!=-1)
    {
        translator.load("ru");
    }
    app.installTranslator(&translator);

    WindowField window;
    //размеры окна
    window.setBaseSize(800,600);
    window.setGeometry(50,50,800,600);
    window.show();

    return app.exec();
}
