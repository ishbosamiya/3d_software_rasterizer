#include <iostream>
#include "display.h"
#include "stars3D.h"
#include "bitmap.h"
#include "renderContext.h"
#include "vector4f.h"
using namespace std;

float toRadians(float angle) {
    return angle * 3.141592653 / 180.0;
}

int main(int argc, char *argv[]) {
    //for fps counter
    int step = 1;
    float average_fps = 0;
    //creating the display and updating it
    Display display("Software Rendering", 1280, 720);

    Bitmap texture(32, 32, 3);
    texture.generateNoise();

    //Stars3D stars(4096, 64.0f, 0.1f, 72);
    //Random Triangle Vertices
    Vertex minYVert(Vector4f(-1, -1, 0, 1),
                    Vector4f(0, 0, 0, 0));
    Vertex midYVert(Vector4f(0, 1, 0, 1),
                    Vector4f(0.5, 1, 0, 0));
    Vertex maxYVert(Vector4f(1, -1, 0, 1),
                    Vector4f(1, 0, 0, 0));

    //setting up perspective
    Matrix4f projection;
    projection.initPerspective(toRadians(70.0), (float)display.render_context.getWidth()/(float)display.render_context.getHeight(), 0.1, 1000.0);

    //test code for rotation of triangle
    float rot_counter = 0.0;

    unsigned long long int previous_time = SDL_GetTicks();
    //checking for any events that have occurred
    SDL_Event event;
    while(true) {
        //getting the time difference between each frame
        unsigned long long int current_time = SDL_GetTicks();
        double delta = current_time - previous_time;
        previous_time = current_time;
        average_fps += delta;
        //cout << "FPS: " << 1000.0/delta << endl;
        if(step % 60 == 0) {
            cout << "FPS: " << 1000.0/((float)average_fps/(float)step) << endl;
            if(step >= 600) {
                average_fps = (float)average_fps/(float)step;
                step = 1;
            }
        }
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
        //Setting up matrices for translation and rotation and the final transform
        rot_counter += delta / 500.0;
        Matrix4f translation;
        translation.initTranslation(0, 0, 3.0);
        Matrix4f rotation;
        rotation.initRotation(0, rot_counter, 0);
        Matrix4f transform_ = projection.mul(translation.mul(rotation));

        display.render_context.clear(0);

        //Creating the triangle
        display.render_context.fillTriangle(minYVert.transform(transform_), midYVert.transform(transform_), maxYVert.transform(transform_), texture);
        //display.render_context.fillWireframe(minYVert.transform(transform_), midYVert.transform(transform_), maxYVert.transform(transform_), 255, 0, 0, 2);

        display.renderImage();

        //updating the window
        SDL_UpdateWindowSurface(display.window);
        //SDL_Delay(10);
        step++;
    }
}
