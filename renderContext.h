#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "bitmap.h"
#include "vertex.h"

class RenderContext: public Bitmap
{
    int *m_scan_buffer;
    void scanConvertLine(Vertex minYVert, Vertex maxYVert, int whichSide);

    public:
        RenderContext();
        RenderContext(unsigned int width, unsigned int height, unsigned int channels = 3);
        void initialize(unsigned int width, unsigned int height, unsigned int channels = 3);
        void drawScanBuffer(int yCoord, int xMin, int xMax);
        void fillShape(int yMin, int yMax);
        void scanConvertTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, int handiness);
        void fillTriangle(Vertex v1, Vertex v2, Vertex v3);
        ~RenderContext();

    protected:
};

#endif // RENDERCONTEXT_H
