//Adresse Serveur : 10.5.133.185

#include "libraries.h"

int main(int argc, char *argv[])
{
       QApplication app(argc, argv);

       MainWindow *w = new MainWindow();
       w->show();
       joystick* jo = new joystick ();

       QObject::connect(jo,SIGNAL(signal_sdl_quitter()), w, SLOT(quitterApp()));
       QObject::connect(jo,SIGNAL(signal_sdl_mode_manuel()),w,SLOT(envoieModeManuel()));
       QObject::connect(jo,SIGNAL(signal_sdl_mode_auto()),w,SLOT(envoieModeAuto()));



       return app.exec();
}

