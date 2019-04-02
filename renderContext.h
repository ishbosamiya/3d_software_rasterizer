#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <iostream>
#include <limits>
#include <math.h>

#include "bitmap.h"
#include "vertex.h"
#include "edge.h"
#include "gradients.h"
#include "mesh.h"

class RenderContext: public Bitmap
{
    void scanTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, bool handedness, Bitmap texture);
    void scanEdges(Gradients gradients, Edge &a, Edge &b, bool handedness, Bitmap texture);
    void drawScanLine(Gradients gradients, Edge &left, Edge &right, int j, Bitmap texture);
    void drawWire(Edge edge, int thickness, char r, char g, char b);

    float *m_z_buffer;

    public:
        RenderContext();
        RenderContext(unsigned int width, unsigned int height, unsigned int channels = 3);
        void initialize(unsigned int width, unsigned int height, unsigned int channels = 3);

        void fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap texture, bool back_face_culling);
        void fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap texture, bool wireframe, bool back_face_culling);
        void fillWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness, bool back_face_culling);
        void drawMesh(Mesh mesh, Matrix4f transform, Bitmap texture, bool wireframe, bool back_face_culling);

        void clearDepthBuffer();
        void drawZBuffer();
        RenderContext getNormalizedZBuffer();

        RenderContext getResizedRenderContext(int width, int height);

        ~RenderContext();

    protected:
};

#endif // RENDERCONTEXT_H
