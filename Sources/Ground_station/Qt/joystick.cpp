#include "libraries.h"

joystick::joystick()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)< 0;

    /*SDL_Surface *ecran = NULL;
    ecran = SDL_SetVideoMode(640,480, 32, SDL_HWSURFACE);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,253,108,158));
    SDL_Flip(ecran);*/
    printf("MOCHE AAAAAAAA DEGUEUUUUUUU");
    int SDL_NumJoysticks();
    SDL_JoystickEventState(SDL_ENABLE);

    SDL_Joystick *Joystick; // création du joystick
    Joystick = SDL_JoystickOpen(0); // assignation au numéro 0


    qDebug() << " nb de joystick :" << SDL_NumJoysticks() << " \n ";

    static SDL_Event evenements;


    while (SDL_PollEvent(&evenements))
    {
        if (Joystick != NULL)
        {
            switch (evenements.type)
            {
                case SDL_JOYBUTTONDOWN:
                    switch (evenements.jbutton.button)
                    {
                        case 0: // Quitter l'IHM
                            break;
                        case 1: // quitter le robot (trame quit)
                            break;
                        case 2: // change etat robot (set_state)
                            break;
                        case 3: // change l'etat de l'IA (set_ai)
                            break;
                        case 8: // connecte/deconnecte IHM (pour test pas obligatoire ici)
                            break;

                    }
                    break;
                case SDL_JOYAXISMOTION:
                    if(evenements.jaxis.axis ==0 && evenements.jaxis.value < -32767 && evenements.jaxis.axis ==1 && evenements.jaxis.value = 0)//si la valeur de l'axe 0 est inférieur à -2000 donc si il est vers la gauche
                        qDebug () << "gauche";
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
