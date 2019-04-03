#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <iostream>
#include <limits>
#include <vector>
#include <math.h>
#include <string.h>
#include <algorithm>

#include "bitmap.h"
#include "vertex.h"
#include "edge.h"
#include "gradients.h"
#include "mesh.h"
#include "shader.h"

class RenderContext: public Bitmap
{
    void scanTriangle(Vertex &minYVert, Vertex &midYVert, Vertex &maxYVert, bool handedness, Bitmap &texture, Shader *shader);
    void scanEdges(Gradients &gradients, Edge &a, Edge &b, bool handedness, Bitmap &texture, Shader *shader);
    void drawScanLine(Gradients &gradients, Edge &left, Edge &right, int j, Bitmap &texture, Shader *shader);
    void drawWire(Edge &edge, int thickness, char r, char g, char b);
    void clipPolygonComponent(vector<Vertex> &vertices, int component_index, float component_factor, vector<Vertex> &result);
    bool clipPolygonAxis(vector<Vertex> &vertices, vector<Vertex> &auxillary_vector, int component_index);

    float *m_z_buffer;

    public:
        RenderContext();
        RenderContext(const RenderContext &obj);
        RenderContext(unsigned int width, unsigned int height);
        void initialize(unsigned int width, unsigned int height);

        void fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &texture, bool back_face_culling);
        void fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &texture, bool wireframe, bool back_face_culling);
        void fillWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness, bool back_face_culling);
        void drawMesh(Mesh &mesh, Matrix4f view_projection, Matrix4f transform, Bitmap &texture, bool wireframe, bool back_face_culling);
        void drawTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &texture, bool back_face_culling);
        void drawWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness, bool back_face_culling);
        void drawWire(Vertex v1, Vertex v2, int thickness, char r, char g, char b);

        void clearDepthBuffer();
        void drawZBuffer();
        Bitmap getNormalizedZBuffer();
        void getNormalizedZBuffer(Bitmap &image);

        RenderContext getResizedRenderContext(int width, int height);
        void getResizedRenderContext(RenderContext &image);

        virtual ~RenderContext();

    protected:
};

#endif // RENDERCONTEXT_H
