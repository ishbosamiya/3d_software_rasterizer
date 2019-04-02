#include <iostream>
#include "display.h"
#include "stars3D.h"
#include "bitmap.h"
#include "renderContext.h"
#include "vector4f.h"
#include "mesh.h"
#include "objmodel.h"
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

    Mesh *mesh;
    Mesh mesh_data[5];
    //mesh_data[0].initialize("icosphere.obj");

    mesh = &mesh_data[0];
    bool draw_triangle = true;
    bool draw_wireframe = false;
    bool draw_z_buffer = false;

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
    bool rotation_check = false;
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
                        case SDLK_r:
                            rotation_check = (!rotation_check);
                            break;
                        case SDLK_1:
                            if(mesh_data[0].isInitialized() == false) {
                                mesh_data[0].initialize("icosphere.obj");
                                cout << "icosphere.obj loaded" << endl;
                            }
                            mesh = &mesh_data[0];
                            draw_triangle = false;
                            break;
                        case SDLK_2:
                            if(mesh_data[1].isInitialized() == false) {
                                mesh_data[1].initialize("sphere_high.obj");
                                cout << "sphere_high.obj loaded" << endl;
                            }
                            mesh = &mesh_data[1];
                            draw_triangle = false;
                            break;
                        case SDLK_3:
                            if(mesh_data[2].isInitialized() == false) {
                                mesh_data[2].initialize("monkey0.obj");
                                cout << "monkey0.obj loaded" << endl;
                            }
                            mesh = &mesh_data[2];
                            draw_triangle = false;
                            break;
                        case SDLK_4:
                            if(mesh_data[3].isInitialized() == false) {
                                mesh_data[3].initialize("cylinder.obj");
                                cout << "cylinder.obj loaded" << endl;
                            }
                            mesh = &mesh_data[3];
                            draw_triangle = false;
                            break;
                        case SDLK_5:
                            if(mesh_data[4].isInitialized() == false) {
                                mesh_data[4].initialize("cylinder_hollow.obj");
                                cout << "cylinder_hollow.obj loaded" << endl;
                            }
                            mesh = &mesh_data[4];
                            draw_triangle = false;
                            break;
                        case SDLK_t:
                            draw_triangle = !draw_triangle;
                            break;
                        case SDLK_w:
                            draw_wireframe = !draw_wireframe;
                            break;
                        case SDLK_z:
                            draw_z_buffer = !draw_z_buffer;
                        default:
                            break;
                    }
                    break;
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
        if(rotation_check == true) {
            rotation.initRotation(rot_counter, rot_counter + 90.0, rot_counter);
        }
        else {
            rotation.initRotation(0, rot_counter, 0);
        }
        Matrix4f transform_ = projection.mul(translation.mul(rotation));

        display.render_context.clear(0);
        display.render_context.clearDepthBuffer();

        //display.render_context.fillWireframe(minYVert.transform(transform_), midYVert.transform(transform_), maxYVert.transform(transform_), 255, 0, 0, 2);
        if(draw_triangle == true) {
          display.render_context.fillTriangle(minYVert.transform(transform_), midYVert.transform(transform_), maxYVert.transform(transform_), texture, draw_wireframe, false);
        }
        else {
            display.render_context.drawMesh(*mesh, transform_, texture, draw_wireframe, false);
        }

        if(draw_z_buffer == true) {
            display.render_context.drawZBuffer();
        }
        display.renderImage();

        //updating the window
        SDL_UpdateWindowSurface(display.window);
        //SDL_Delay(10);
        step++;
    }
}
