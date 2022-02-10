#include "basewidget.h"
#include <QApplication>
#include <QIcon>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    baseWidget w;
    w.show();

    return a.exec();
}
