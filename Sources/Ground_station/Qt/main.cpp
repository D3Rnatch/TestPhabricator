//Adresse Serveur : 10.5.133.185

#include "libraries.h"

#undef main

int main(int argc, char *argv[])
{
       QApplication app(argc, argv);

       MainWindow *w = new MainWindow();
       w->show();
      joystick* j = new joystick ();


       //Connections des touches du Joystick vers l'interface
       QObject::connect(j,SIGNAL(signal_sdl_quitter()), w, SLOT(quitterApp()));
       QObject::connect(j,SIGNAL(signal_sdl_mode_manuel()),w,SLOT(envoieModeManuel()));
       QObject::connect(j,SIGNAL(signal_sdl_mode_auto()),w,SLOT(envoieModeAuto()));

       //Transferts de données du Joystick vers les serveur
       w->setJoystick_x(j->getJoystick_x());
       w->setJoystick_y(j->getJoystick_y());
       w->setJoystick_t(j->getJoystick_t());

       return app.exec();
}

