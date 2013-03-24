#include <QApplication>

#include "window.h"
#include "params.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // save params
    argCount=argc;
    argValue=argv;

    //gui
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 500);
    mainWindow.show();
    return app.exec();
}
