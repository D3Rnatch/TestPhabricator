//Adresse Serveur : 10.5.133.185

#include "libraries.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
       QApplication app(argc, argv);

       MainWindow w;
       w.show();

       return app.exec();
}

