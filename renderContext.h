#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "bitmap.h"

class RenderContext: public Bitmap
{
    int *m_scan_buffer;

    public:
        RenderContext();
        RenderContext(unsigned int width, unsigned int height, unsigned int channels = 3);
        void initialize(unsigned int width, unsigned int height, unsigned int channels = 3);
        void drawScanBuffer(int yCoord, int xMin, int xMax);
        void fillShape(int yMin, int yMax);
        ~RenderContext();

    protected:
};

#endif // RENDERCONTEXT_H
