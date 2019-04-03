#include <iostream>

#include "display.h"
#include "bitmap.h"
#include "renderContext.h"
#include "vector4f.h"
#include "mesh.h"
#include "objmodel.h"
#include "transform.h"
#include "camera.h"
#include "input.h"
using namespace std;

const int width = 1280*0.9;
const int height = 720*0.9;

float toRadians(float angle) {
    return angle * 3.141592653 / 180.0;
}

int main(int argc, char *argv[]) {
    //for fps counter
    int step = 1;
    float average_fps = 0;
    //creating the display and updating it
    Display z_buffer_display("z Buffer", width/5, height/5, 0, 0);
    Display display("Software Rendering", width, height);

    Bitmap texture(32, 32, 3);
    texture.generateNoise();
    Bitmap texture2(15, 15, 3);
    texture2.generateNoise();

    Mesh *mesh;
    Mesh mesh_data[9];
    mesh = mesh_data[2].initialize("monkey0.obj");
    Transform transform_mesh(Vector4f(0.0, 0.0, 3.0, 1.0));
    Mesh terrainMesh;
    terrainMesh.initialize("terrain0.obj");
    Transform terrain_transform(Vector4f(0.0, -1.0, 0.0, 1.0));

    bool draw_wireframe = false;
    bool draw_z_buffer = false;
    bool show_cursor = false;
    bool capture_mouse = true;

    //setting up perspective
    Matrix4f projection;
    projection.initPerspective(toRadians(70.0), (float)display.render_context.getWidth()/(float)display.render_context.getHeight(), 0.1, 1000.0);
    Camera camera(projection, width, height);
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
    SDL_ShowCursor(SDL_DISABLE);
    SDL_CaptureMouse(SDL_TRUE);
    Input input(&display);
    float standard_mov_amt = 5.0 * 0.0002;
    while(true) {
        //getting the time difference between each frame
        unsigned long long int current_time = SDL_GetTicks();
        double delta = current_time - previous_time;
        previous_time = current_time;
        average_fps += delta;
        //cout << "FPS: " << 1000.0/delta << endl;
        if(step % 10 == 0) {
            cout << "FPS: " << 1000.0/((float)average_fps/(float)step) << endl;
            if(step >= 100) {
                average_fps = (float)average_fps/(float)step;
                step = 1;
            }
        }
        float mov_amt = standard_mov_amt * delta;
        //input data
        input.event(event, capture_mouse);
        if(input.isPressed(KEY_ESCAPE)) {
            SDL_Quit();
            return 0;
        }
        if(input.isPressed(KEY_1)) {
            mesh = mesh_data[0].initialize("icosphere.obj");
        }
        if(input.isPressed(KEY_2)) {
            mesh = mesh_data[1].initialize("sphere_high.obj");
        }
        if(input.isPressed(KEY_3)) {
            mesh = mesh_data[2].initialize("monkey0.obj");
        }
        if(input.isPressed(KEY_4)) {
            mesh = mesh_data[3].initialize("cylinder.obj");
        }
        if(input.isPressed(KEY_5)) {
            mesh = mesh_data[4].initialize("cylinder_hollow.obj");
        }
        if(input.isPressed(KEY_6)) {
            mesh = mesh_data[5].initialize("man.obj");
        }
        if(input.isPressed(KEY_7)) {
            mesh = mesh_data[6].initialize("teapot.obj");
        }
        if(input.isPressed(KEY_8)) {
            mesh = mesh_data[7].initialize("monster_low.obj");
        }
        if(input.isPressed(KEY_9)) {
            mesh = mesh_data[8].initialize("monster_high.obj");
        }
        if(input.isPressed(KEY_F)) {
            if(draw_z_buffer == true && draw_wireframe == false) {
                draw_z_buffer = !draw_z_buffer;
            }
            draw_wireframe = !draw_wireframe;
        }
        if(input.isPressed(KEY_Z)) {
            if(draw_wireframe == true && draw_z_buffer == false) {
                draw_wireframe = !draw_wireframe;
            }
            draw_z_buffer = !draw_z_buffer;
        }
        if(input.isPressed(KEY_C)) {
            if(show_cursor) {
                SDL_ShowCursor(SDL_DISABLE);
                SDL_CaptureMouse(SDL_TRUE);
                show_cursor = false;
                capture_mouse = true;
            }
            else {
                SDL_ShowCursor(SDL_ENABLE);
                SDL_CaptureMouse(SDL_FALSE);
                show_cursor = true;
                capture_mouse = false;
            }
        }
        if(input.isPressed(KEY_UP)) {
            standard_mov_amt += 0.003;
        }
        if(input.isPressed(KEY_DOWN)) {
            standard_mov_amt -= 0.003;
        }
        input.getMouse(mouse_x, mouse_y);
        camera.update(input, delta, mouse_x, mouse_y, mov_amt);

        //Program Logic after this
        //basic display wiping
        display.render_context.clear(0);
        display.render_context.clearDepthBuffer();

        //mesh drawing
        Matrix4f vp = camera.getViewProjection();
        display.render_context.drawMesh(*mesh, vp.mul(transform_mesh.getTransformation()), texture, draw_wireframe, false);
        display.render_context.drawMesh(terrainMesh, vp.mul(terrain_transform.getTransformation()), texture2, draw_wireframe, false);

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
