#include "bitmap.h"
#include <stdlib.h>

Bitmap::Bitmap() {
}

Bitmap::Bitmap(const Bitmap &obj) {
    m_width = obj.m_width;
    m_height = obj.m_height;
    m_channels = obj.m_channels;
    m_components = new Colour[m_width * m_height];
    for(int i = 0; i < m_width * m_height * m_channels; i++) {
        m_components[i] = obj.m_components[i];
    }
    m_image_surface = obj.m_image_surface;
}

Bitmap::Bitmap(unsigned int width, unsigned int height) {
    //basic initialization of the values
    initialize(width, height);
}

Bitmap Bitmap::getResizedBitmap(int width, int height) {
    Bitmap image(width, height);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            int source_x = x * getWidth() / width;
            int source_y = y * getHeight() / height;
            image.copyPixel(x, y, source_x, source_y, *this);
        }
    }
    return image;
}

void Bitmap::getResizedBitmap(Bitmap &image) {
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

Bitmap::Bitmap(char *file_name) {
    SDL_Surface *image = SDL_LoadBMP(file_name);
    if(image == NULL) {
        cout << "Could Not Load Image!!!" << endl;
        initialize(512, 512);
        generateNoise();
        return;
    }
    m_image_surface = image;

    m_width = image->w;
    m_height = image->h;
    cout << "width: " << m_width << endl;
    cout << "height: " << m_height << endl;

    m_components = new Colour[m_width * m_height];
    SDL_LockSurface(image);

    //Uint32 *pixels = (Uint32 *)image->pixels;
    Uint32 *pixels = (Uint32 *)m_image_surface->pixels;
    cout << pixels[m_width * m_height - 2501] << endl;

    //copy_n(pixels, m_width * m_height, (Uint32 *)m_components);
    //memcpy((void *)m_components, (void *)pixels, m_width * m_height);
    memcpy((void *)m_components, (void *)pixels, m_width * m_height * sizeof(*m_components));
//    for(int i = 0; i < m_width * m_height; i++) {
//        m_components[i].packed = pixels[i];
//    }

    //storing as ppm
    char *temp = new char[1000];
    strcpy(temp, file_name);
    strcat(temp, ".ppm");
    ofstream fout;
    fout.open(temp);
    fout << "P3" << endl;
    fout << m_width << " " << m_height << endl;
    fout << "255" << endl;
    for(int y = 0; y < m_height; y++) {
        for(int x = 0; x < m_width; x++) {
            int index = x + y * m_width;
            fout << (int)m_components[index].r << " ";
            fout << (int)m_components[index].g << " ";
            fout << (int)m_components[index].b << " ";
        }
        fout << endl;
    }

    fout.close();
    fout.open("temp_data.txt");
    for(int i = 0; i < m_width * m_height; i++) {
        long int component_temp;
        memcpy(&component_temp, &m_components[i], sizeof(long int));
        bitset<32> p(component_temp);
        fout << p << endl;
    }

    m_channels = 4;
    cout << "Image has been Loaded" << endl;

    SDL_UnlockSurface(image);
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

void Bitmap::initialize(unsigned int width, unsigned int height) {
    //basic initialization of the values
    m_width = width;
    m_height = height;
    m_channels = 4;
    m_components = new Colour[m_width * m_height];
    m_image_surface = NULL;
}

SDL_Surface* Bitmap::getSurface() {
    //return m_image_surface;
    //to get the sdl surface to the image
    SDL_FreeSurface(m_image_surface);
    m_image_surface = SDL_CreateRGBSurfaceFrom((void *)m_components,
                                                    m_width,
                                                    m_height,
                                                    m_channels * 8,
                                                    m_width * m_channels,
                                                    0x000000FF,
                                                    0x0000FF00,
                                                    0x00FF0000,
                                                    0xFF000000);
    return m_image_surface;
}

void Bitmap::clear(char shade) {
    //memset(m_components, shade, m_width * m_height * m_channels);
    Colour colour;
    colour.fill(shade);
    fill_n(m_components, m_width * m_height, colour);
}

void Bitmap::drawPixel(int x, int y, Colour colour) {
    //changing colour of single pixel based on specified location of the pixel
    if(x >= 0 && x < m_width && y >=0 && y < m_height) {
        int index = (x + y * m_width);
        m_components[index] = colour;
    }
}

void Bitmap::copyPixel(int dest_X, int dest_Y, int src_X, int src_Y, Bitmap &src) {
    int destIndex = (dest_X + dest_Y * m_width);
    int srcIndex = (src_X + src_Y * src.getWidth());
    m_components[destIndex] = src.getComponent(srcIndex);
}

void Bitmap::getPixel(int x, int y, Colour &colour) {
    int index = (x + y * m_width);
    colour = m_components[index];
}

void Bitmap::generateNoise() {
    //adding random r, g, b values to each pixel
    srand(10);
    for(int x = 0; x < m_width; x++) {
        for(int y = 0; y < m_height; y++) {
            Colour temp_colour;
            //for craziness, enable alpha as some random other number
//            temp_colour.fill((char)(rand()%255), (char)(rand()%255), (char)(rand()%255), (char)(150));
            temp_colour.fill((char)(rand()%255), (char)(rand()%255), (char)(rand()%255), 255);
            drawPixel(x, y, temp_colour);
        }
    }
}

Bitmap::~Bitmap() {
    delete [] m_components;
    SDL_FreeSurface(m_image_surface);
}
