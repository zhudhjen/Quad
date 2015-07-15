#include "welcomewindow.h"
#include <QApplication>
#include <GL/glut.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeWindow w;
    w.show();

    return a.exec();
}
