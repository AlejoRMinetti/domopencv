#include "Login/login.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // comienzo en pantalla de login
    LogIn w;
    w.show();

    return a.exec();
}
