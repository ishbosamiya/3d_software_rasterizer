#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include "bitmap.h"
#include "renderContext.h"

class Display
{
    unsigned int width;
    unsigned int height;
    char title[32];

    public:
        SDL_Window *window;
        SDL_Surface *screenSurface;
        RenderContext render_context;
        Display(char *title, unsigned int width, unsigned int height);
        //refreshing the window for image updating
        void updateWindow();
        //to render the image in memory to the window
        void renderImage();
        ~Display();

    protected:
};

#endif // DISPLAY_H
