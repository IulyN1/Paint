#include "GUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI* gui = new GUI;
    gui->show();
    return a.exec();
}
