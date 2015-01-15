//Adresse Serveur : 10.5.133.185

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QFont>
#include <QIcon>
#include <QCoreApplication>
#include <QDebug>

#include <QtWidgets>
#include <QtNetwork>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();

}

