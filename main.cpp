#include <iostream>

#include "display.h"
#include "bitmap.h"
#include "renderContext.h"
#include "vector4f.h"
#include "mesh.h"
#include "objmodel.h"
#include "transform.h"
#include "camera.h"
using namespace std;

float toRadians(float angle) {
    return angle * 3.141592653 / 180.0;
}

int main(int argc, char *argv[]) {
    const int width = 1280;
    const int height = 720;
    //for fps counter
    int step = 1;
    float average_fps = 0;
    //creating the display and updating it
    Display z_buffer_display("z Buffer", width/5, height/5, 0, 0);
    Display display("Software Rendering", width, height);

    Bitmap texture(32, 32, 3);
    texture.generateNoise();

    Mesh *mesh;
    Mesh mesh_data[7];
    mesh = mesh_data[2].initialize("monkey0.obj");
    Transform transform_mesh(Vector4f(0.0, 0.0, 3.0, 1.0));
    Mesh terrainMesh;
    terrainMesh.initialize("terrain0.obj");
    Transform terrain_transform(Vector4f(0.0, -1.0, 0.0, 1.0));

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
    Matrix4f translation;
    Matrix4f rotation;
    Matrix4f scale;

    Camera camera(projection);
    camera.rotate(Vector4f(0, 1, 0, 1), toRadians(180));
    camera.move(Vector4f(0, 0, -1, 1), -7);

    //test code for rotation of triangle
    bool rotation_check = false;
    float rot_counter = 0.0;

    unsigned long long int previous_time = SDL_GetTicks();
    //checking for any events that have occurred
    SDL_Event event;
    int mouse_x;
    int mouse_y;
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
        if(SDL_WaitEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    return 0;
                case SDL_KEYDOWN:
                    camera.update(event, delta, mouse_x, mouse_y);
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            SDL_Quit();
                            return 0;
                        case SDLK_1:
                            mesh = mesh_data[0].initialize("icosphere.obj");
                            draw_triangle = false;
                            break;
                        case SDLK_2:
                            mesh = mesh_data[1].initialize("sphere_high.obj");
                            draw_triangle = false;
                            break;
                        case SDLK_3:
                            mesh = mesh_data[2].initialize("monkey0.obj");
                            draw_triangle = false;
                            break;
                        case SDLK_4:
                            mesh = mesh_data[3].initialize("cylinder.obj");
                            draw_triangle = false;
                            break;
                        case SDLK_5:
                            mesh = mesh_data[4].initialize("cylinder_hollow.obj");
                            draw_triangle = false;
                            break;
                        case SDLK_6:
                            mesh = mesh_data[5].initialize("man.obj");
                            draw_triangle = false;
                            break;
                        case SDLK_7:
                            mesh = mesh_data[6].initialize("teapot.obj");
                            draw_triangle = false;
                            break;
                        case SDLK_f:
                            if(draw_z_buffer == true && draw_wireframe == false) {
                                draw_z_buffer = !draw_z_buffer;
                            }
                            draw_wireframe = !draw_wireframe;
                            break;
                        case SDLK_z:
                            if(draw_wireframe == true && draw_z_buffer == false) {
                                draw_wireframe = !draw_wireframe;
                            }
                            draw_z_buffer = !draw_z_buffer;
                        default:
                            break;
                    }
                    break;
//                default:
//                    break;
                case SDL_MOUSEMOTION:
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;
                    camera.update(event, delta, mouse_x, mouse_y);
                    if(mouse_x >= display.getWidth() - 1) {
                        SDL_WarpMouseInWindow(display.window, 0, mouse_y);
                        //SDL_WarpMouseGlobal(0, mouse_y);
                    }
                    else if(mouse_x <= 1) {
                        SDL_WarpMouseInWindow(display.window, display.getWidth() - 1, mouse_y);
                        //SDL_WarpMouseGlobal(display.getWidth() - 1, mouse_y);
                    }
                    if(mouse_y >= display.getHeight() - 1) {
                        SDL_WarpMouseInWindow(display.window, mouse_x, 0);
                        //SDL_WarpMouseGlobal(mouse_x, 0);
                    }
                    else if(mouse_y <= 1) {
                        SDL_WarpMouseInWindow(display.window, mouse_x, display.getHeight() - 1);
                        //SDL_WarpMouseGlobal(mouse_x, display.getHeight() - 1);
                    }
                    break;
            }
        }

        //Program Logic after this
        //basic display wiping
        display.render_context.clear(0);
        display.render_context.clearDepthBuffer();

        //mesh drawing
        Matrix4f vp = camera.getViewProjection();
        Matrix4f temp = vp.mul(transform_mesh.getTransformation());
        display.render_context.drawMesh(*mesh, temp, texture, draw_wireframe, false);
        temp = vp.mul(terrain_transform.getTransformation());
        display.render_context.drawMesh(terrainMesh, temp, texture, draw_wireframe, false);

        //depth map displaying
        Bitmap z_buffer = display.render_context.getNormalizedZBuffer();
        if(draw_z_buffer == true) {
            display.renderImage(z_buffer);
        }
        else {
            display.renderImage();
        }
        Bitmap z_buffer_small = z_buffer.getResizedBitmap(z_buffer_display.getWidth(), z_buffer_display.getHeight());
        z_buffer_display.renderImage(z_buffer_small);

        //updating the window
        SDL_UpdateWindowSurface(display.window);
        SDL_UpdateWindowSurface(z_buffer_display.window);
        step++;
    }
}
