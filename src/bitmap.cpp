#include "bitmap.h"
#include <stdlib.h>
Bitmap::Bitmap() {
    //ctor
}

Bitmap::Bitmap(unsigned int width, unsigned int height, unsigned int channels) {
    //basic initialization of the values
    m_width = width;
    m_height = height;
    m_channels = channels;
    m_components = new char[width * height * channels];
}

void Bitmap::initialize(unsigned int width, unsigned int height, unsigned int channels) {
    //copy of overloaded constructor Bitmap(unsigned int width, unsigned int height, unsigned int channels)

    //basic initialization of the values
    m_width = width;
    m_height = height;
    m_channels = channels;
    m_components = new char[width * height * channels];
}

SDL_Surface* Bitmap::getSurface() {
    //to get the sdl surface to the image
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)m_components,
                                                    m_width,
                                                    m_height,
                                                    m_channels * 8,
                                                    m_width * m_channels,
                                                    0x0000FF,
                                                    0x00FF00,
                                                    0xFF0000,
                                                    0);
    return surface;
}

void Bitmap::clear(char shade) {
    //running through all the pixel's of the image to set it to one single shade
    for(int i = 0; i < m_width * m_height * m_channels; i = i + m_channels) {
        m_components[i + 0] = shade;
        m_components[i + 1] = shade;
        m_components[i + 2] = shade;
    }
}

void Bitmap::drawPixel(int x, int y, char r, char g, char b) {
    //changing colour of single pixel based on specified location of the pixel
    int index = (x + y * m_width) * m_channels;
    m_components[index + 0] = r;
    m_components[index + 1] = g;
    m_components[index + 2] = b;
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
    //dtor
}
