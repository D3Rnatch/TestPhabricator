#include "libraries.h"


joystick::joystick()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)< 0;

    int SDL_NumJoysticks();
    SDL_JoystickEventState(SDL_ENABLE);

    SDL_Joystick *Joystick; // création du joystick
    Joystick = SDL_JoystickOpen(0); // assignation au numéro 0


    qDebug() << " nb de joystick :" << SDL_NumJoysticks() << " \n ";

    static SDL_Event evenements;

    float coeff = 0.0003518509476;
    float coeff_t = 0.0015259254738;

    while (SDL_WaitEvent(&evenements))
    {
        if (Joystick != NULL)
        {
            switch (evenements.type)
            {

                case SDL_JOYBUTTONDOWN:
                    switch (evenements.jbutton.button)
                        {
                            case 0: // Quitter l'IHM
                            qDebug ()<<"\nQuitter l'IHM \n";
                            emit signal_sdl_quitter();
                                break;

                            case 1: // quitter le robot (trame quit)
                            qDebug ()<<"Envoyer le message quitter\n";
                            emit signal_sdl_quitter();
                                break;

                            case 2: // change etat robot (set_state)
                            qDebug ()<<"\nchange etat\n";
                            emit signal_sdl_mode_manuel();
                                break;

                            case 3: // change l'etat de l'IA (set_ai)
                            qDebug ()<<"\nChange l'IA\n";
                            emit signal_sdl_mode_auto();
                                break;

                            default:
                               qDebug()<<" touche non enregistrée";
                                break;
                        }
                    break;

                case SDL_JOYAXISMOTION:

                        if(evenements.jaxis.axis == 0) //axe des absisses
                        {
                            x = evenements.jaxis.value;
                            qDebug()<<"\nValeur de x :"<< x <<"\n";

                            //Calcule de la conversion 32767 to 20
                            joystick_x = coeff*float(x)+10;


                            qDebug()<<"\nValeur de joystick_x :"<<joystick_x<<"\n";
                            qDebug()<<"\nValeur de joystick_y :"<<joystick_y<<"\n";
                            qDebug()<<"\nValeur de joystick_x :"<<joystick_t<<"\n";

                            if(evenements.jaxis.value < 0)
                                    qDebug()<<"\nA gauche !: "<<evenements.jaxis.value<<"\n";

                                else if(evenements.jaxis.value >0)
                                    qDebug()<<"\nA droite !: "<<evenements.jaxis.value<<"\n";

                                else
                                   qDebug()<<"\ntout droit: "<<evenements.jaxis.value<<"\n";
                          }

                        else if(evenements.jaxis.axis == 1) //axe des ordonnées
                        {

                            y = evenements.jaxis.value;
                            qDebug()<<"\nValeur de y :"<<y<<"\n";


                            //Calcule de la conversion 32767 to 20
                            joystick_y = coeff*float(y)+10;

                            qDebug()<<"\nValeur de joystick_x :"<<joystick_x<<"\n";
                            qDebug()<<"\nValeur de joystick_y :"<<joystick_y<<"\n";
                            qDebug()<<"\nValeur de joystick_t :"<<joystick_t<<"\n";

                                if(evenements.jaxis.value < 0)

                                    qDebug()<<"\nEn avant !"<<evenements.jaxis.value<<"\n";

                                else if(evenements.jaxis.value >0)
                                    qDebug()<<"\nEn arrière !"<<evenements.jaxis.value<<"\n";

                                else
                                   qDebug()<<"\ntout droit: "<<evenements.jaxis.value<<"\n";
                        }

                        else if(evenements.jaxis.axis == 3) //axe de lévitation
                        {
                            t = evenements.jaxis.value;
                            qDebug()<<"\nValeur de t :"<<t<<"\n";


                            //Calcule de la conversion 32767 to 100

                            joystick_t = coeff_t*float(-t)+50;
                            qDebug()<<"\nValeur de joystick_t :"<<joystick_t<<"\n";
                            qDebug()<<"\nValeur de joystick_x :"<<joystick_x<<"\n";
                            qDebug()<<"\nValeur de joystick_y :"<<joystick_y<<"\n";


                        }

                      break;

                 default :
                            qDebug()<<"\nTouche non connue\n";
                    break;

        }
      }
    }


    SDL_JoystickClose(Joystick);
   // pause();
    qDebug () << "exit failure \n ";
    SDL_Quit();



    //return EXIT_FAILURE;
}


void joystick::pause()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}

