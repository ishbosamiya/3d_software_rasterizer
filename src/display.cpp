#include "display.h"
#include <string.h>
#include <iostream>
using namespace std;

Display::Display(char *title, unsigned int width, unsigned int height) {
    //basic assignment of values
    strcpy(this->title, title);
    this->width = width;
    this->height = height;
    window = NULL;
    screenSurface = NULL;

    //making sure that sdl has initialized
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Failed to initialize SDL!!! " << SDL_GetError() << endl;
        return;
    }

    //creating the window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        cout << "Failed to create window!!! " << SDL_GetError() << endl;
        return;
    }

    //making the image
    bitmap.initialize(width, height, 3);
    bitmap.clear(25);
    bitmap.generateNoise();
    //getting the sdl surface for the image in memory
    image_surface = bitmap.getSurface();

    //making the window surface
    screenSurface = SDL_GetWindowSurface(window);
}

void Display::updateWindow() {
    //checking for any events that have occurred
    SDL_Event event;
    while(true) {
        if(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            return;
                        default:
                            break;
                    }
                default:
                    break;

            }
        }

        //Program Logic after this
        SDL_BlitSurface(image_surface, NULL, screenSurface, NULL);

        //updating the window
        SDL_UpdateWindowSurface(window);
    }
}

Display::~Display() {
    //deleting the context of window
    SDL_DestroyWindow(window);
}
