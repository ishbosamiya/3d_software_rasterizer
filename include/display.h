#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <bitmap.h>

class Display
{
    unsigned int width;
    unsigned int height;
    char title[32];
    SDL_Window *window;
    SDL_Surface *screenSurface;
    Bitmap bitmap;
    SDL_Surface *image_surface;

    public:
        Display(char *title, unsigned int width, unsigned int height);
        //refreshing the window for image updating
        void updateWindow();
        ~Display();

    protected:
};

#endif // DISPLAY_H
