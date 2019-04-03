#include "input.h"

Input::Input(Display *display) {
    m_key_pressed = new bool[KEY_COUNT];
    for(int i = 0; i < KEY_COUNT; i++) {
        m_key_pressed[i] = false;
    }
    mouse_x = 0;
    mouse_y = 0;
    mouse_prev_x = SDL_WINDOWPOS_CENTERED;
    mouse_prev_y = SDL_WINDOWPOS_CENTERED;
    mouse_diff_x = 0;
    mouse_diff_y = 0;
    this->display = NULL;
    this->display = display;
}

void Input::event(SDL_Event &event, bool capture_mouse) {
//    //SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
//    SDL_GetMouseState(&mouse_x, &mouse_y);
//    cout << "x: " << mouse_x << " y: " << mouse_y << endl;
//    mouse_diff_x = mouse_x - mouse_prev_x;
//    mouse_diff_y = mouse_y - mouse_prev_y;
//    mouse_prev_x = mouse_x;
//    mouse_prev_y = mouse_y;

    //this works
    SDL_GetRelativeMouseState(&mouse_diff_x, &mouse_diff_y);

    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if(key_state[SDL_SCANCODE_0]) {
        m_key_pressed[KEY_0] = true;
    }
    else {
        m_key_pressed[KEY_0] = false;
    }
    if(key_state[SDL_SCANCODE_1]) {
        m_key_pressed[KEY_1] = true;
    }
    else {
        m_key_pressed[KEY_1] = false;
    }
    if(key_state[SDL_SCANCODE_2]) {
        m_key_pressed[KEY_2] = true;
    }
    else {
        m_key_pressed[KEY_2] = false;
    }
    if(key_state[SDL_SCANCODE_3]) {
        m_key_pressed[KEY_3] = true;
    }
    else {
        m_key_pressed[KEY_3] = false;
    }
    if(key_state[SDL_SCANCODE_4]) {
        m_key_pressed[KEY_4] = true;
    }
    else {
        m_key_pressed[KEY_4] = false;
    }
    if(key_state[SDL_SCANCODE_5]) {
        m_key_pressed[KEY_5] = true;
    }
    else {
        m_key_pressed[KEY_5] = false;
    }
    if(key_state[SDL_SCANCODE_6]) {
        m_key_pressed[KEY_6] = true;
    }
    else {
        m_key_pressed[KEY_6] = false;
    }
    if(key_state[SDL_SCANCODE_7]) {
        m_key_pressed[KEY_7] = true;
    }
    else {
        m_key_pressed[KEY_7] = false;
    }
    if(key_state[SDL_SCANCODE_8]) {
        m_key_pressed[KEY_8] = true;
    }
    else {
        m_key_pressed[KEY_8] = false;
    }
    if(key_state[SDL_SCANCODE_9]) {
        m_key_pressed[KEY_9] = true;
    }
    else {
        m_key_pressed[KEY_9] = false;
    }

    if(key_state[SDL_SCANCODE_A]) {
        m_key_pressed[KEY_A] = true;
    }
    else {
        m_key_pressed[KEY_A] = false;
    }
    if(key_state[SDL_SCANCODE_B]) {
        m_key_pressed[KEY_B] = true;
    }
    else {
        m_key_pressed[KEY_B] = false;
    }
    if(key_state[SDL_SCANCODE_C]) {
        m_key_pressed[KEY_C] = true;
    }
    else {
        m_key_pressed[KEY_C] = false;
    }
    if(key_state[SDL_SCANCODE_D]) {
        m_key_pressed[KEY_D] = true;
    }
    else {
        m_key_pressed[KEY_D] = false;
    }
    if(key_state[SDL_SCANCODE_E]) {
        m_key_pressed[KEY_E] = true;
    }
    else {
        m_key_pressed[KEY_E] = false;
    }
    if(key_state[SDL_SCANCODE_F]) {
        m_key_pressed[KEY_F] = true;
    }
    else {
        m_key_pressed[KEY_F] = false;
    }
    if(key_state[SDL_SCANCODE_G]) {
        m_key_pressed[KEY_G] = true;
    }
    else {
        m_key_pressed[KEY_G] = false;
    }
    if(key_state[SDL_SCANCODE_H]) {
        m_key_pressed[KEY_H] = true;
    }
    else {
        m_key_pressed[KEY_H] = false;
    }
    if(key_state[SDL_SCANCODE_I]) {
        m_key_pressed[KEY_I] = true;
    }
    else {
        m_key_pressed[KEY_I] = false;
    }
    if(key_state[SDL_SCANCODE_J]) {
        m_key_pressed[KEY_J] = true;
    }
    else {
        m_key_pressed[KEY_J] = false;
    }
    if(key_state[SDL_SCANCODE_K]) {
        m_key_pressed[KEY_K] = true;
    }
    else {
        m_key_pressed[KEY_K] = false;
    }
    if(key_state[SDL_SCANCODE_L]) {
        m_key_pressed[KEY_L] = true;
    }
    else {
        m_key_pressed[KEY_L] = false;
    }
    if(key_state[SDL_SCANCODE_M]) {
        m_key_pressed[KEY_M] = true;
    }
    else {
        m_key_pressed[KEY_M] = false;
    }
    if(key_state[SDL_SCANCODE_N]) {
        m_key_pressed[KEY_N] = true;
    }
    else {
        m_key_pressed[KEY_N] = false;
    }
    if(key_state[SDL_SCANCODE_O]) {
        m_key_pressed[KEY_O] = true;
    }
    else {
        m_key_pressed[KEY_O] = false;
    }
    if(key_state[SDL_SCANCODE_P]) {
        m_key_pressed[KEY_P] = true;
    }
    else {
        m_key_pressed[KEY_P] = false;
    }
    if(key_state[SDL_SCANCODE_Q]) {
        m_key_pressed[KEY_Q] = true;
    }
    else {
        m_key_pressed[KEY_Q] = false;
    }
    if(key_state[SDL_SCANCODE_R]) {
        m_key_pressed[KEY_R] = true;
    }
    else {
        m_key_pressed[KEY_R] = false;
    }
    if(key_state[SDL_SCANCODE_S]) {
        m_key_pressed[KEY_S] = true;
    }
    else {
        m_key_pressed[KEY_S] = false;
    }
    if(key_state[SDL_SCANCODE_T]) {
        m_key_pressed[KEY_T] = true;
    }
    else {
        m_key_pressed[KEY_T] = false;
    }
    if(key_state[SDL_SCANCODE_U]) {
        m_key_pressed[KEY_U] = true;
    }
    else {
        m_key_pressed[KEY_U] = false;
    }
    if(key_state[SDL_SCANCODE_V]) {
        m_key_pressed[KEY_V] = true;
    }
    else {
        m_key_pressed[KEY_V] = false;
    }
    if(key_state[SDL_SCANCODE_W]) {
        m_key_pressed[KEY_W] = true;
    }
    else {
        m_key_pressed[KEY_W] = false;
    }
    if(key_state[SDL_SCANCODE_X]) {
        m_key_pressed[KEY_X] = true;
    }
    else {
        m_key_pressed[KEY_X] = false;
    }
    if(key_state[SDL_SCANCODE_Y]) {
        m_key_pressed[KEY_Y] = true;
    }
    else {
        m_key_pressed[KEY_Y] = false;
    }
    if(key_state[SDL_SCANCODE_Z]) {
        m_key_pressed[KEY_Z] = true;
    }
    else {
        m_key_pressed[KEY_Z] = false;
    }

    if(key_state[SDL_SCANCODE_ESCAPE]) {
        m_key_pressed[KEY_ESCAPE] = true;
    }
    else {
        m_key_pressed[KEY_ESCAPE] = false;
    }
    if(key_state[SDL_SCANCODE_UP]) {
        m_key_pressed[KEY_UP] = true;
    }
    else {
        m_key_pressed[KEY_UP] = false;
    }
    if(key_state[SDL_SCANCODE_DOWN]) {
        m_key_pressed[KEY_DOWN] = true;
    }
    else {
        m_key_pressed[KEY_DOWN] = false;
    }
    if(key_state[SDL_SCANCODE_LEFT]) {
        m_key_pressed[KEY_LEFT] = true;
    }
    else {
        m_key_pressed[KEY_LEFT] = false;
    }
    if(key_state[SDL_SCANCODE_RIGHT]) {
        m_key_pressed[KEY_RIGHT] = true;
    }
    else {
        m_key_pressed[KEY_RIGHT] = false;
    }
    if(key_state[SDL_SCANCODE_LCTRL]) {
        m_key_pressed[KEY_CTRL] = true;
    }
    else {
        m_key_pressed[KEY_CTRL] = false;
    }
    if(key_state[SDL_SCANCODE_LALT]) {
        m_key_pressed[KEY_ALT] = true;
    }
    else {
        m_key_pressed[KEY_ALT] = false;
    }
    if(key_state[SDL_SCANCODE_LSHIFT]) {
        m_key_pressed[KEY_SHIFT] = true;
    }
    else {
        m_key_pressed[KEY_SHIFT] = false;
    }

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}



Input::~Input() {
    delete [] m_key_pressed;
}
