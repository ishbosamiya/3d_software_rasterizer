#include "bitmap.h"
#include <stdlib.h>

Bitmap::Bitmap() {
    m_width = 0;
    m_height = 0;
    m_channels = 0;
    m_components = nullptr;
    m_image_surface = NULL;
    m_file_name = nullptr;
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
    m_file_name = nullptr;
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

    initialize(file_name);
}

void Bitmap::initialize(char *file_name) {
    m_width = 0;
    m_height = 0;
    m_channels = 0;
    m_components = nullptr;
    m_image_surface = NULL;
    m_file_name = nullptr;

    if(m_width > 0) {
        m_width = 0;
        m_height = 0;
        m_channels = 0;
        delete [] m_components;
        m_components = nullptr;
        SDL_FreeSurface(m_image_surface);
        m_image_surface = NULL;
        delete [] m_file_name;
        m_file_name = nullptr;
    }

    unsigned char *pixels = stbi_load(file_name, &m_width, &m_height, &m_channels, STBI_rgb_alpha);
    m_channels = STBI_rgb_alpha;

    this->m_file_name = file_name;

    if(!pixels) {
        cout << "Image Could Not Be Loaded" << endl;
        initialize(32, 32);
        generateNoise();
        return;
    }

    cout << "Width: " << m_width << endl;
    cout << "Height: " << m_height << endl;

    initialize(m_width, m_height);

    memcpy((void *)m_components, (void *)pixels, m_width * m_height * m_channels);

    m_image_surface = getSurface();

    cout << file_name << " Has Been Loaded" << endl;
    delete [] pixels;
}

void Bitmap::initialize(unsigned int width, unsigned int height) {
    //basic initialization of the values
    m_width = width;
    m_height = height;
    m_channels = 4;
    m_components = new Colour[m_width * m_height];
    m_image_surface = NULL;
    m_file_name = nullptr;
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

Colour Bitmap::getPixel(int x, int y) {
//    if(x > m_width || y > m_height || x < 0 || y < 0) {
//        Colour pink;
//        pink.fill(255, 0, 255);
//        return pink;
//    }
    if(x < 0) {
        x *= -1;
    }
    if(y < 0) {
        y *= -1;
    }
    x = x % m_width;
    y = y % m_height;
    int index = (x + y * m_width);
    return m_components[index];
}

Colour Bitmap::getPixel(Vector4f uv) {
    return getPixel(uv.getX() * m_width, uv.getY() * m_height);
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
    delete [] m_file_name;
}
