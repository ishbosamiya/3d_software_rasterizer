#include "display.h"
#include <string.h>
#include <iostream>
using namespace std;

Display::Display(char *title, unsigned int width, unsigned int height, int position_x, int position_y) {
    //basic assignment of values
    strcpy(this->title, title);
    this->width = width;
    this->height = height;
    window = NULL;
    screen_surface = NULL;

    //making sure that sdl has initialized
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Failed to initialize SDL!!! " << SDL_GetError() << endl;
        return;
    }

    //creating the window
    if(position_x == -1) {
        position_x = SDL_WINDOWPOS_CENTERED;
    }
    if(position_y == -1) {
        position_y = SDL_WINDOWPOS_CENTERED;
    }
    window = SDL_CreateWindow(title, position_x, position_y, width, height, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        cout << "Failed to create window!!! " << SDL_GetError() << endl;
        return;
    }

    //making the image
    render_context.initialize(width, height);
    render_context.clear(25);
    render_context.generateNoise();

    //making the window surface
    screen_surface = SDL_GetWindowSurface(window);
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
        SDL_BlitSurface(render_context.getSurface(), NULL, screen_surface, NULL);

        //updating the window
        SDL_UpdateWindowSurface(window);
    }
}

void Display::renderImage(){
    SDL_BlitSurface(render_context.getSurface(), NULL, screen_surface, NULL);
}

void Display::renderImage(RenderContext &image){
    SDL_BlitSurface(image.getSurface(), NULL, screen_surface, NULL);
}

void Display::renderImage(Bitmap &image){
    SDL_BlitSurface(image.getSurface(), NULL, screen_surface, NULL);
}

Display::~Display() {
    //deleting the context of window
    cout << "Freeing window in Display" << endl;
    SDL_DestroyWindow(window);
    cout << "Freeing screen_surface in Display" << endl;
    SDL_FreeSurface(screen_surface);
}
