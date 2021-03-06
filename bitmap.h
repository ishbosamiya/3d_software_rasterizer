#ifndef BITMAP_H
#define BITMAP_H

#include <limits>
#include <iostream>
#include <string.h>
#include <fstream>
#include <bitset>
#include <algorithm>

#include "SDL.h"
#include "colour.h"
#include "vector4f.h"

#include "stb_image.h"

using namespace std;

class Bitmap
{
    char *m_file_name;

    public:
        Bitmap();
        Bitmap(const Bitmap &obj);
        Bitmap(char *m_file_name);
        Bitmap(unsigned int width, unsigned int height);
        virtual void initialize(unsigned int width, unsigned int height);
        virtual void initialize(char *m_file_name);
        //to get the sdl surface of the image
        SDL_Surface* getSurface();
        //to clear out the image to one single shade of colour
        void clear(char shade);
        //changing colour of single pixel based on specified location of the pixel
        void drawPixel(int x, int y, Colour colour);
        //to generate a noisy image
        void generateNoise();
        //for drawing pixel from a defined texture map
        void copyPixel(int dest_X, int dest_Y, int src_X, int src_Y, Bitmap &src);

        Colour getPixel(int x, int y);
        Colour getPixel(Vector4f uv);

        Colour getComponent(int index) { return m_components[index];}

        Bitmap getResizedBitmap(int width, int height);
        void getResizedBitmap(Bitmap &image);

        Uint32 getPixel32( SDL_Surface *surface, int x, int y );

        int getWidth() {
            return m_width;
        }
        int getHeight() {
            return m_height;
        }
        virtual ~Bitmap();

    protected:
        int m_width;
        int m_height;
        int m_channels;
        Colour *m_components;
        SDL_Surface *m_image_surface;
};

#endif // BITMAP_H
