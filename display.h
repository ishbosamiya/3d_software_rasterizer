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
        Display(char *title, unsigned int width, unsigned int height, int position_x = -1, int position_y = -1);
        //refreshing the window for image updating
        void updateWindow();
        //to render the image in memory to the window
        void renderImage();
        void renderImage(RenderContext &image);
        void renderImage(Bitmap &image);

        RenderContext getRenderContext() { return render_context;}
        int getWidth() { return width;}
        int getHeight() { return height;}
        ~Display();

    protected:
};

#endif // DISPLAY_H
