#include "bitmap.h"
#include <stdlib.h>

Bitmap::Bitmap() {
}

Bitmap::Bitmap(const Bitmap &obj) {
    m_width = obj.m_width;
    m_height = obj.m_height;
    m_channels = obj.m_channels;
    m_components = new char[m_width * m_height * m_channels];
    for(int i = 0; i < m_width * m_height * m_channels; i++) {
        m_components[i] = obj.m_components[i];
    }
    m_image_surface = obj.m_image_surface;
}

Bitmap::Bitmap(unsigned int width, unsigned int height, unsigned int channels) {
    //basic initialization of the values
    initialize(width, height, channels);
}

Bitmap Bitmap::getResizedBitmap(int width, int height) {
    Bitmap image(width, height, 3);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            int source_x = x * getWidth() / width;
            int source_y = y * getHeight() / height;
            image.copyPixel(x, y, source_x, source_y, *this);
        }
    }
    return image;
}

Bitmap Bitmap::getResizedBitmap(Bitmap &image) {
    int width = image.getWidth();
    int height = image.getHeight();
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            int source_x = x * getWidth() / width;
            int source_y = y * getHeight() / height;
            image.copyPixel(x, y, source_x, source_y, *this);
        }
    }
}

Uint32 Bitmap::getPixel32( SDL_Surface *surface, int x, int y ) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    int index = y * surface->w + x;

    Uint32 value = pixels[index];
    //Get the requested pixel
    return value;
}

//Bitmap::Bitmap(char *file_name) {
//    have_surface = true;
//    SDL_Surface *image = SDL_LoadBMP(file_name);
//    if(image == NULL) {
//        cout << "Could Not Load Image!!!" << endl;
//        initialize(32, 32, 3);
//        generateNoise();
//        return;
//    }
//    m_image_surface = image; //SDL_ConvertSurface(image, gSceenSurface->format, NULL);
//
//    m_width = image->w;
//    m_height = image->h;
//    m_channels = 3;
//
//    m_components = new char[m_width * m_height * m_channels];
//
//    for(int x = 0; x < m_width; x++) {
//        for(int y = 0; y < m_height; y++) {
//            unsigned char r, g, b;
//            SDL_GetRGB(getPixel32(m_image_surface, x, y), m_image_surface->format, &r, &g, &b);
//            drawPixel(x, y, r, g, b);
//            cout << "Image Loaded" << endl;
//        }
//    }
//
//    have_surface = true;
//}

void Bitmap::initialize(unsigned int width, unsigned int height, unsigned int channels) {
    //basic initialization of the values
    m_width = width;
    m_height = height;
    m_channels = channels;
    m_components = new char[width * height * channels];
    m_image_surface = NULL;
}

SDL_Surface* Bitmap::getSurface() {
    //to get the sdl surface to the image
    SDL_FreeSurface(m_image_surface);
    m_image_surface = SDL_CreateRGBSurfaceFrom((void *)m_components,
                                                    m_width,
                                                    m_height,
                                                    m_channels * 8,
                                                    m_width * m_channels,
                                                    0x0000FF,
                                                    0x00FF00,
                                                    0xFF0000,
                                                    0);
    return m_image_surface;
}

void Bitmap::clear(char shade) {
    //running through all the pixel's of the image to set it to one single shade
    for(int i = 0; i < m_width * m_height * m_channels; i++) {
        m_components[i] = shade;
    }
}

void Bitmap::drawPixel(int x, int y, char r, char g, char b) {
    //changing colour of single pixel based on specified location of the pixel
    int index = (x + y * m_width) * m_channels;
    m_components[index + 0] = r;
    m_components[index + 1] = g;
    m_components[index + 2] = b;
}

void Bitmap::copyPixel(int dest_X, int dest_Y, int src_X, int src_Y, Bitmap &src) {
    int destIndex = (dest_X + dest_Y * m_width) * m_channels;
    int srcIndex = (src_X + src_Y * src.getWidth()) * m_channels;
    m_components[destIndex + 0] = src.getComponent(srcIndex + 0);
    m_components[destIndex + 1] = src.getComponent(srcIndex + 1);
    m_components[destIndex + 2] = src.getComponent(srcIndex + 2);
}

void Bitmap::getPixel(int x, int y, char &r, char &g, char &b) {
    int index = (x + y * m_width) * m_channels;
    r = m_components[index + 0];
    g = m_components[index + 1];
    b = m_components[index + 2];
}

void Bitmap::generateNoise() {
    //adding random r, g, b values to each pixel
    srand(10);
    for(int x = 0; x < m_width; x++) {
        for(int y = 0; y < m_height; y++) {
            drawPixel(x, y, rand()%255, rand()%255, rand()%255);
        }
    }
}

Bitmap::~Bitmap() {
    delete [] m_components;
    SDL_FreeSurface(m_image_surface);
    //delete m_image_surface;
}
