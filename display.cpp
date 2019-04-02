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

    //making the window surface
    screenSurface = SDL_GetWindowSurface(window);
}

void Display::updateWindow() {
    unsigned long long int previous_time = SDL_GetTicks();
    //checking for any events that have occurred
    SDL_Event event;
    while(true) {
        //getting the time difference between each frame
        unsigned long long int current_time = SDL_GetTicks();
        double delta = current_time - previous_time;
        previous_time = current_time;
        cout << "FPS: " << 1000.0/delta << endl;
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
        SDL_BlitSurface(bitmap.getSurface(), NULL, screenSurface, NULL);

        //updating the window
        SDL_UpdateWindowSurface(window);
    }
}

void Display::renderImage(){
    SDL_BlitSurface(bitmap.getSurface(), NULL, screenSurface, NULL);
}

Display::~Display() {
    //deleting the context of window
    SDL_DestroyWindow(window);
}
