#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "libraries.h"

class joystick: public QObject
{
    Q_OBJECT

    public:
        joystick();
        void pause();
        float joystick_x;
        float joystick_y;
        float joystick_t;

    signals:
        void signal_sdl_mode_manuel();
        void signal_sdl_quitter();
        void signal_sdl_mode_auto();

    private:
        float x;
        float y;
        float t;

};

#endif // JOYSTICK_H
