#include "mainwindow.h"
#include "glwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget* glw = new GLWidget();
    glw->resize(800,600);
    glw->show();
    return a.exec();
}
