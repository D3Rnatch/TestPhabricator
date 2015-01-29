//Adresse Serveur : 10.5.133.185

#include "libraries.h"

#undef main

int main(int argc, char *argv[])
{
       QApplication app(argc, argv);


       MainWindow *w = new MainWindow();
       w->show();

       return app.exec();
}

