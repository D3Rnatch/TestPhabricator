#include "libraries.h"

joystick::joystick()
{



    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)< 0;

    /*SDL_Surface *ecran = NULL;
    ecran = SDL_SetVideoMode(640,480, 32, SDL_HWSURFACE);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,253,108,158));
    SDL_Flip(ecran);*/
    //printf("MOCHE AAAAAAAA DEGUEUUUUUUU");
    int SDL_NumJoysticks();
    SDL_JoystickEventState(SDL_ENABLE);

    SDL_Joystick *Joystick; // création du joystick
    Joystick = SDL_JoystickOpen(0); // assignation au numéro 0


    qDebug() << " nb de joystick :" << SDL_NumJoysticks() << " \n ";

    static SDL_Event evenements;





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

                            int x = evenements.jaxis.value;
                            qDebug()<<"\nValeur de x :"<<x<<"\n";
                            int X = ((20*65534)/x);

                            qDebug()<<"\nValeur de X :"<<X<<"\n";

                            if(evenements.jaxis.value < 0)
                                    qDebug()<<"\nA gauche !: "<<evenements.jaxis.value<<"\n";


                                else if(evenements.jaxis.value >0)
                                    qDebug()<<"\nA droite !: "<<evenements.jaxis.value<<"\n";

                                else
                                   qDebug()<<"\ntout droit: "<<evenements.jaxis.value<<"\n";

                          }

                        else if(evenements.jaxis.axis == 1) //axe des ordonnées
                        {

                            int y = evenements.jaxis.value;
                            qDebug()<<"\nValeur de y :"<<y<<"\n";

                            //int Y = (20*65534)/y;

                            //qDebug()<<"\nValeur de Y :"<<Y<<"\n";

                                if(evenements.jaxis.value < 0)
                                    qDebug()<<"\nEn avant !"<<evenements.jaxis.value<<"\n";

                                else if(evenements.jaxis.value >0)
                                    qDebug()<<"\nEn arrière !"<<evenements.jaxis.value<<"\n";

                                else
                                   qDebug()<<"\ntout droit: "<<evenements.jaxis.value<<"\n";
                        }

                        else if(evenements.jaxis.axis == 3) //axe de lévitation
                        {

                            int t = evenements.jaxis.value;
                            qDebug()<<"\nValeur de t :"<<t<<"\n";

                            //int T = (100*65534)/t;

                            //qDebug()<<"\nValeur de X :"<<T<<"\n";

                                //if(evenements.jaxis.value < 0)
                                    //qDebug()<<"\nEn avant !"<<evenements.jaxis.value<<"\n";

                                //else if(evenements.jaxis.value >0)
                                    //qDebug()<<"\nEn arrière !"<<evenements.jaxis.value<<"\n";

                                //else
                                   //qDebug()<<"\ntout droit: "<<evenements.jaxis.value<<"\n";
                        }

                      break;

                 default :
                            qDebug()<<"\nErreur\n";
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

