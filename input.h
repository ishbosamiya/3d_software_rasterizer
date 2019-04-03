#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include "display.h"

enum {
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    KEY_CTRL,
    KEY_SHIFT,
    KEY_ALT,
    KEY_ESCAPE,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,

    MOUSE_LEFT_CLICK,
    MOUSE_RIGHT_CLICK,
    MOUSE_SCROLL_UP,
    MOUSE_SCROLL_DOWN,

    KEY_COUNT,
};

class Input
{
    bool *m_key_pressed;
    int mouse_x;
    int mouse_y;
    int mouse_prev_x;
    int mouse_prev_y;
    Display *display;
    public:
        Input(Display *display);
        void event(SDL_Event &event, bool capture_mouse);
        void getMouse(int &x, int &y) { x = mouse_x; y = mouse_y;}
        void getMouseDifference(int &x, int &y) { x = mouse_x - mouse_prev_x; y = mouse_y - mouse_prev_y;}
        void pressKey(int key) { m_key_pressed[key] = true;}
        void unpressKey(int key) { m_key_pressed[key] = false;}
        bool isPressed(int key) { return m_key_pressed[key];}
        virtual ~Input();

    protected:
};

#endif // INPUT_H
