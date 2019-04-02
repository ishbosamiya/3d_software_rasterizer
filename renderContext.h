#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "bitmap.h"
#include "vertex.h"
#include "edge.h"
#include "gradients.h"

class RenderContext: public Bitmap
{
    void scanTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, bool handedness, Bitmap texture);
    void scanEdges(Gradients gradients, Edge &a, Edge &b, bool handedness, Bitmap texture);
    void drawScanLine(Gradients gradients, Edge &left, Edge &right, int j, Bitmap texture);
    void drawWire(Edge edge, int thickness, char r, char g, char b);

    public:
        RenderContext();
        RenderContext(unsigned int width, unsigned int height, unsigned int channels = 3);
        void initialize(unsigned int width, unsigned int height, unsigned int channels = 3);
        void fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap texture);
        void fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap texture, bool wireframe);
        void fillWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness = 2);
        ~RenderContext();

    protected:
};

#endif // RENDERCONTEXT_H
