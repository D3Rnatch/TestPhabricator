#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "libraries.h"

class joystick: public QObject
{
    Q_OBJECT

    public:
        joystick();
        void pause();

    signals:
        void signal_sdl_mode_manuel();
        void signal_sdl_quitter();
        void signal_sdl_mode_auto();

    private:
        float joystick_x;
};

#endif // JOYSTICK_H
