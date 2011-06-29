#include <QApplication>
#include "windowfield.h"

int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    WindowField window;
    window.show();

    return app.exec();
}
