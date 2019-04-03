#define STB_IMAGE_IMPLEMENTATION //needed for stb_image

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
#include "SDL_ttf.h"
#include "shader.h"
using namespace std;

const float resolution_factor = 0.6;
const int width = 1280 * resolution_factor;
const int height = 720 * resolution_factor;

float toRadians(float angle) {
    return angle * 3.141592653 / 180.0;
}

void drawText(Display &display, TTF_Font *font, SDL_Color text_colour);
void int_to_string(int value, char *result);
void float_to_string(float value, char *result);

int main(int argc, char *argv[]) {
//    Display temp_display("Temp Display", width, height);
//    Bitmap temp_bitmap("image.bmp");
//    SDL_Event temp_event;
//    while(true) {
//        if(SDL_PollEvent(&temp_event)) {
//            switch(temp_event.type) {
//                case SDL_QUIT:
//                    return 0;
//                case SDL_KEYDOWN:
//                    switch(temp_event.key.keysym.sym) {
//                        case SDLK_ESCAPE:
//                            return 0;
//                        default:
//                            break;
//                    }
//                default:
//                    break;
//            }
//        }
//        temp_display.renderImage(temp_bitmap);
//        SDL_UpdateWindowSurface(temp_display.window);
//    }

    float alpha = 0, beta = 20, gamma = 0;
    Shader::base_colour.fill(255, 255, 255);

    //For Text On Screen
    TTF_Init();
    //TTF_Font* font = TTF_OpenFont("arial.ttf", 14);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 0.021605 * height);
    if(font == NULL) {
        cout << "Unable To Load Font" << endl;
        return -1;
    }
    SDL_Color text_colour = { 255, 255, 255 };

    //for fps counter
    int step = 1;
    float average_fps = 0;
    //creating the display and updating it
    Display z_buffer_display("z Buffer", width/5, height/5, 0, 0);
    Display display("Software Rendering", width, height);

    Bitmap texture(32, 32);
    texture.generateNoise();
    Bitmap texture2(8, 8);
    texture2.generateNoise();
    Bitmap frank_tex("frank_DIF.png");

    Mesh *mesh;
    Mesh mesh_data[9];
    mesh = mesh_data[2].initialize("monkey0.obj");
    Transform transform_mesh(Vector4f(0.0, 0.0, 0.0, 1.0));
    Mesh terrainMesh;
    terrainMesh.initialize("terrain0.obj");
    Transform terrain_transform(Vector4f(0.0, -1.0, 0.0, 1.0));

    bool draw_wireframe = false;
    bool draw_z_buffer = false;
    bool show_cursor = false;
    bool capture_mouse = true;
    bool draw_help = true;
    bool turn_light = true;

    //setting up perspective
    Matrix4f projection;
    projection.initPerspective(toRadians(70.0), (float)display.render_context.getWidth()/(float)display.render_context.getHeight(), 0.1, 1000.0);
    Camera camera(projection, width, height);
    camera.rotate(Vector4f(0, 1, 0, 1), toRadians(180));
    camera.move(Vector4f(0, 0, -1, 1), -4);

    //test code for rotation of triangle
    bool rotation_check = false;
    float rot_counter = 0.0;

    unsigned long long int previous_time = SDL_GetTicks();
    //checking for any events that have occurred
    SDL_Event event;
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_ShowCursor(SDL_DISABLE);
    SDL_CaptureMouse(SDL_TRUE);
    //SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(display.window, display.getWidth() / 2, display.getHeight() / 2);
    Input input(&display);
    float standard_mov_amt = 5.0 * 0.0002;
    while(true) {
        SDL_PumpEvents();
        //getting the time difference between each frame
        unsigned long long int current_time = SDL_GetTicks();
        double delta = current_time - previous_time;
        previous_time = current_time;
        //For FPS on Screen
        char fps_string[32];
        float fps = 1000.0/(delta);
        float_to_string(fps, fps_string);
        char fps_string_final[32];
        strcpy(fps_string_final, "FPS: ");
        strcat(fps_string_final, fps_string);
        SDL_Surface *text_fps_surface = NULL;
        if(fps < 10) {
            SDL_Color fps_colour = {255, 25, 25};
            text_fps_surface = TTF_RenderText_Blended(font, fps_string_final, fps_colour);
        }
        else {
            SDL_Color fps_colour = {25, 255, 25};
            text_fps_surface = TTF_RenderText_Blended(font, fps_string_final, fps_colour);
        }
        SDL_Rect text_fps_location = {0, 0, 0, 0};

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
                SDL_SetRelativeMouseMode(SDL_TRUE);
                show_cursor = false;
                capture_mouse = true;
            }
            else {
                SDL_ShowCursor(SDL_ENABLE);
                SDL_CaptureMouse(SDL_FALSE);
                SDL_SetRelativeMouseMode(SDL_FALSE);
                show_cursor = true;
                capture_mouse = false;
            }
        }
        if(input.isPressed(KEY_UP)) {
            standard_mov_amt += 0.001;
        }
        if(input.isPressed(KEY_DOWN)) {
            standard_mov_amt -= 0.001;
            if(standard_mov_amt < 0) {
                standard_mov_amt = 0.001;
            }
        }
        if(input.isPressed(KEY_T)) {
            draw_help = !draw_help;
        }
        //for the lighting
        if(input.isPressed(KEY_K)) {
            beta += 5;
        }
        if(input.isPressed(KEY_I)) {
            beta -= 5;
        }
        if(input.isPressed(KEY_L)) {
            turn_light = !turn_light;
        }
        if(input.isPressed(KEY_M)) {
            if(input.isPressed(KEY_CTRL)) {
                if(Shader::shader_type > -1) {
                    Shader::shader_type = -2;
                }
                else {
                    Shader::shader_type = -1;
                }
            }
            Shader::shader_type++;
        }
        if(input.isPressed(KEY_LEFT)) {
            Shader::ambient_factor += -0.01;
            Shader::ambient_factor = max(Shader::ambient_factor, 0.0f);
        }
        if(input.isPressed(KEY_RIGHT)) {
            Shader::ambient_factor += 0.01;
            Shader::ambient_factor = min(Shader::ambient_factor, 0.1f);
        }


        //input.getMouseDifference(mouse_x, mouse_y);
        camera.update(input, delta, mov_amt);

        //Program Logic after this
        //basic display wiping
        display.render_context.clear(0);
        display.render_context.clearDepthBuffer();

        //mesh drawing
        Matrix4f vp = camera.getViewProjection();
        if(mesh == &mesh_data[7] || mesh == &mesh_data[8]) {
            display.render_context.drawMesh(*mesh, vp, transform_mesh.getTransformation(), frank_tex, draw_wireframe, false);
        }
        else {
            display.render_context.drawMesh(*mesh, vp, transform_mesh.getTransformation(), texture, draw_wireframe, false);
        }
        display.render_context.drawMesh(terrainMesh, vp, terrain_transform.getTransformation(), texture2, draw_wireframe, false);

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

        if(draw_help) {
            drawText(display, font, text_colour);
        }
        //Drawing the fps counter
        SDL_BlitSurface(text_fps_surface, NULL, display.screen_surface, &text_fps_location);

        //updating the window
        SDL_UpdateWindowSurface(display.window);
        SDL_UpdateWindowSurface(z_buffer_display.window);
        step++;
        SDL_FreeSurface(text_fps_surface);
        if(turn_light) {
            alpha += 15;
            gamma = 90 - alpha;
        }
        Shader::light_dir.initialize(cos(toRadians(alpha)), sin(toRadians(beta)), cos(toRadians(gamma)));
    }
}

void drawText(Display &display, TTF_Font *font, SDL_Color text_colour) {
    int no_of_textBoxes = 14;
    SDL_Surface *textSurface[no_of_textBoxes];
    for(int i = 0; i < no_of_textBoxes; i++) {
        textSurface[i] = NULL;
    }
    int text_number = 0;
    SDL_Rect textLocation[no_of_textBoxes];
    int window_size_x = display.getWidth();
    int window_size_y = display.getHeight();

    //Actual Text Information
    //Left Side
    textSurface[text_number] = TTF_RenderText_Blended(font, "WASD To Move", text_colour);
    textLocation[text_number] = {0.2 * window_size_x, 0.07 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "Mouse To Turn", text_colour);
    textLocation[text_number] = {0.2 * window_size_x, 0.11 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "1-9 For Different Models", text_colour);
    textLocation[text_number] = {0.2 * window_size_x, 0.15 * window_size_y, 0, 0};

    //Right Side
    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "F To Toggle Wireframe Mode", text_colour);
    textLocation[text_number] = {0.65 * window_size_x, 0.07 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "Z To Toggle Depth View", text_colour);
    textLocation[text_number] = {0.65 * window_size_x, 0.11 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "C To Toggle Mouse Capture", text_colour);
    textLocation[text_number] = {0.65 * window_size_x, 0.15 * window_size_y, 0, 0};

    //Middle
    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "M To Toggle Different Shaders", text_colour);
    textLocation[text_number] = {0.4215 * window_size_x, 0.07 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "L To Stop Or Start Turning Of Light", text_colour);
    textLocation[text_number] = {0.408 * window_size_x, 0.11 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "K and I To Rotate Light", text_colour);
    textLocation[text_number] = {0.442 * window_size_x, 0.15 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "UP Arrow To Increase Camera Speed", text_colour);
    textLocation[text_number] = {0.4 * window_size_x, 0.19 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "DOWN Arrow To Decrease Camera Speed", text_colour);
    textLocation[text_number] = {0.386 * window_size_x, 0.23 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "Left And Right Keys To Change Ambient Occlusion", text_colour);
    textLocation[text_number] = {0.36 * window_size_x, 0.27 * window_size_y, 0, 0};

    text_number++;
    textSurface[text_number] = TTF_RenderText_Blended(font, "T TO Toggle Help", text_colour);
    textLocation[text_number] = {0.45 * window_size_x, 0.31 * window_size_y, 0, 0};

    //Bottom Left
    //Shader Information
        text_number++;
        if(Shader::shader_type % Shader::no_of_shaders == 0) {
            textSurface[text_number] = TTF_RenderText_Blended(font, "Shader: Texture + Lighting Smooth", text_colour);
        }
        else if(Shader::shader_type % Shader::no_of_shaders == 1) {
            textSurface[text_number] = TTF_RenderText_Blended(font, "Shader: Lighting Smooth Normals", text_colour);
        }
        else if(Shader::shader_type % Shader::no_of_shaders == 2) {
            textSurface[text_number] = TTF_RenderText_Blended(font, "Shader: Lighting Flat Normals", text_colour);
        }
        else if(Shader::shader_type % Shader::no_of_shaders == 3) {
            textSurface[text_number] = TTF_RenderText_Blended(font, "Shader: Texture + Lighting Flat", text_colour);
        }
        else if(Shader::shader_type % Shader::no_of_shaders == 4) {
            textSurface[text_number] = TTF_RenderText_Blended(font, "Shader: Normal Information Via Colour", text_colour);
        }
        else if(Shader::shader_type == -1) {
            textSurface[text_number] = TTF_RenderText_Blended(font, "Shader: Unlit", text_colour);
        }
        textLocation[text_number] = {0.01 * window_size_x, 0.96 * window_size_y, 0, 0};

    // Pass zero for width and height to draw the whole surface
    for(int i = 0; i < no_of_textBoxes; i++) {
        SDL_BlitSurface(textSurface[i], NULL, display.screen_surface, &textLocation[i]);
    }
    //freeing memory
    for(int i = 0; i < no_of_textBoxes; i++) {
        SDL_FreeSurface(textSurface[i]);
    }
}

void float_to_string(float value, char *result) {
    ofstream fout;
    fout.open("float_to_string_temp.txt");
    fout << value;
    fout.close();
    ifstream fin;
    fin.open("float_to_string_temp.txt");
    fin.getline(result, 30);
    fin.close();
}

void int_to_string(int value, char *result) {
    char temp[32];
    int iterator = 0;
    while(value>0) {
        temp[iterator] = value % 10 + (int)'0';
        value /= 10;
        iterator++;
    }
    temp[iterator] = '\0';
    result[iterator] = '\0';
    iterator--;
    int reverse_iterator = 0;
    while(iterator >= 0) {
        result[reverse_iterator] = temp[iterator];
        reverse_iterator++;
        iterator--;
    }
}
