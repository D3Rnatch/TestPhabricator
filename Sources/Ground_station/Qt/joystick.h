/*#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "libraries.h"

class joystick: public QObject
{
    Q_OBJECT

    public:
        joystick();
        void pause();

        float getJoystick_x(){return joystick_x;}
        float getJoystick_y(){return joystick_y;}
        float getJoystick_t(){return joystick_t;}

        void setJoystick_x(float){joystick_x = x;}
        void setJoystick_y(float){joystick_y = y;}
        void setJoystick_t(float){joystick_t = t;}

    signals:
        void signal_sdl_mode_manuel();
        void signal_sdl_quitter();
        void signal_sdl_mode_auto();


    private:

        float x;
        float y;
        float t;


        float joystick_x;
        float joystick_y;
        float joystick_t;

};

#endif // JOYSTICK_H

*/
