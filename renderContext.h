#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "bitmap.h"
#include "vertex.h"
#include "edge.h"

class RenderContext: public Bitmap
{
    int *m_scan_buffer;
    void scanConvertLine(Vertex minYVert, Vertex maxYVert, int whichSide);
    void scanTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, bool handedness);
    void scanEdges(Edge &a, Edge &b, bool handedness);
    void drawScanLine(Edge &left, Edge &right, int j);
    void drawWire(Edge edge, int thickness, char r, char g, char b);

    public:
        RenderContext();
        RenderContext(unsigned int width, unsigned int height, unsigned int channels = 3);
        void initialize(unsigned int width, unsigned int height, unsigned int channels = 3);
        void drawScanBuffer(int yCoord, int xMin, int xMax);
        void fillShape(int yMin, int yMax);
        void scanConvertTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, int handiness);
        void fillTriangle(Vertex v1, Vertex v2, Vertex v3);
        void fillTriangle(Vertex v1, Vertex v2, Vertex v3, bool wireframe);
        void fillWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness = 2);
        ~RenderContext();

    protected:
};

#endif // RENDERCONTEXT_H
