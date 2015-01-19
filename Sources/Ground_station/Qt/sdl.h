#ifndef SDL
#define SDL

char windowid[64];
#ifdef Q_WS_WIN
    sprintf(windowid, "SDL_WINDOWID=0x%lx", reinterpret_cast<qlonglong>(winId()));
#elif defined Q_WS_X11
    sprintf(windowid, "SDL_WINDOWID=0x%lx", winId());
#else
    qFatal("Fatal: cast du winId() inconnu pour votre plate-forme; toute information est la bienvenue!");
#endif
SDL_putenv(windowid);
// Initialisation du système vidéo de SDL
SDL_Init(SDL_INIT_VIDEO);
screen = SDL_SetVideoMode(width(), height(), 32, SDL_SWSURFACE);

setAttribute(Qt::WA_PaintOnScreen);
setAttribute(Qt::WA_NoSystemBackground);


#define main SDL_main
/* The prototype for the application's main() function */
extern C_LINKAGE int SDL_main(int argc, char *argv[]);


#endif // SDL

