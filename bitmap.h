#ifndef BITMAP_H
#define BITMAP_H

#include <SDL.h>

class Bitmap
{
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_channels;
    char *m_components;
    public:
        Bitmap();
        Bitmap(unsigned int width, unsigned int height, unsigned int channels = 3);
        virtual void initialize(unsigned int width, unsigned int height, unsigned int channels = 3);
        //to get the sdl surface of the image
        SDL_Surface* getSurface();
        //to clear out the image to one single shade of colour
        void clear(char shade);
        //changing colour of single pixel based on specified location of the pixel
        void drawPixel(int x, int y, char r, char g, char b);
        //to generate a noisy image
        void generateNoise();
        //for drawing pixel from a defined texture map
        void copyPixel(int dest_X, int dest_Y, int src_X, int src_Y, Bitmap src);

        char getComponent(int index) { return m_components[index];}

        int getWidth() {
            return m_width;
        }
        int getHeight() {
            return m_height;
        }
        ~Bitmap();

    protected:
};

#endif // BITMAP_H
