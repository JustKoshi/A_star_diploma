#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QDir>

#if !TESTS
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
	
    return a.exec();
}

#endif
