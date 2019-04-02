#include <iostream>
#include "display.h"
#include "stars3D.h"
#include "bitmap.h"
#include "renderContext.h"
using namespace std;

int main(int argc, char *argv[]) {
    //creating the display and updating it
    Display display("Software Rendering", 1280, 720);

    Stars3D stars(4096, 64.0f, 0.1f, 72);
    Vertex minYVert(100, 100);
    Vertex midYVert(0, 200);
    Vertex maxYVert(80, 300);
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
                    return 0;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            return 0;
                        default:
                            break;
                    }
                default:
                    break;

            }
        }

        //Program Logic after this
        display.render_context.clear(0);

        //Creating the triangle
        display.render_context.fillTriangle(maxYVert, midYVert, minYVert);

        display.renderImage();

        //updating the window
        SDL_UpdateWindowSurface(display.window);
        //SDL_Delay(10);
    }
}
