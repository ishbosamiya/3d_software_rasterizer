#include "vertex.h"

Vertex::Vertex() {
    m_pos = Vector4f(0, 0, 0, 0);
}

Vertex::Vertex(float x, float y, float z)
{
    m_pos = Vector4f(x ,y, z, 1);
}

Vertex::Vertex(Vector4f pos, Vector4f texCoords, Vector4f normal_smooth, Vector4f normal_flat) {
    initialize(pos, texCoords, normal_smooth, normal_flat);
}

void Vertex::initialize(Vector4f pos, Vector4f texCoords, Vector4f normal_smooth, Vector4f normal_flat) {
    m_pos = pos;
    m_texCoords = texCoords;
    m_normal_smooth = normal_smooth;
    m_normal_flat = normal_flat;
}

//general getter for the x y z and w
float Vertex::get(int index) {
    switch(index) {
        case 0:
            return m_pos.getX();
        case 1:
            return m_pos.getY();
        case 2:
            return m_pos.getZ();
        case 3:
            return m_pos.getW();
        default:
            cout << "Wrong Index In Vertex Getter!!! " << index << endl;
            return 0;
    }
}

//Cross product of the vectors formed by current vertex, b and c
float Vertex::triangleArea(Vertex b, Vertex c) {
    float x1 = b.getX() - m_pos.getX();
    float y1 = b.getY() - m_pos.getY();

    float x2 = c.getX() - m_pos.getX();
    float y2 = c.getY() - m_pos.getY();

    return (float)(x1 * y2 - x2 * y1);
    //return (0.5 * (x1 * y2 - x2 * y1));
}

//to perform a matrix transform to the vertex
Vertex Vertex::transform(Matrix4f transform_) {
    return Vertex(transform_.transform(m_pos), m_texCoords, m_normal_smooth, m_normal_flat);
}

Vertex Vertex::transform(Matrix4f transform_, Matrix4f normal_transform) {
    return Vertex(transform_.transform(m_pos),
                  m_texCoords,
                  normal_transform.transform(m_normal_smooth).normalized(),
                  normal_transform.transform(m_normal_flat).normalized());
}

//To add perspective to the vertex
Vertex Vertex::perspectiveDivide() {
    return Vertex(Vector4f(m_pos.getX()/m_pos.getW(), m_pos.getY()/m_pos.getW(), m_pos.getZ()/m_pos.getW(), m_pos.getW())
                  , m_texCoords, m_normal_smooth, m_normal_flat);
}

Vertex Vertex::lerp(Vertex other, float lerp_amount) {
    return Vertex(m_pos.lerp(other.getPosition(), lerp_amount),
                  m_texCoords.lerp(other.getTexCoords(), lerp_amount),
                  m_normal_smooth.lerp(other.getNormalSmooth(), lerp_amount),
                  m_normal_flat.lerp(other.getNormalFlat(), lerp_amount));
}

bool Vertex::isInsideViewFrustum() {
    return (fabs(m_pos.getX()) <= fabs(m_pos.getW()) &&
            fabs(m_pos.getY()) <= fabs(m_pos.getW()) &&
            fabs(m_pos.getZ()) <= fabs(m_pos.getW()));
}

Vertex::~Vertex()
{
    //dtor
}
