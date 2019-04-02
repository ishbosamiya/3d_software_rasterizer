#ifndef GRADIENTS_H
#define GRADIENTS_H

#include "vector4f.h"
#include "vertex.h"

class Gradients
{
    float *m_texCoordX;
    float *m_texCoordY;
    float *m_one_over_z;

    float m_texCoordX_XStep;
    float m_texCoordX_YStep;
    float m_texCoordY_XStep;
    float m_texCoordY_YStep;
    float m_one_over_z_XStep;
    float m_one_over_z_YStep;

    public:
        Gradients(Vertex minYVert, Vertex midYVert, Vertex maxYVert);
        float getTexCoordX(int loc) { return m_texCoordX[loc];}
        float getTexCoordY(int loc) { return m_texCoordY[loc];}
        float getOneOverZ(int loc) { return m_one_over_z[loc];}
        float getTexCoordX_XStep() { return m_texCoordX_XStep;}
        float getTexCoordX_YStep() { return m_texCoordX_YStep;}
        float getTexCoordY_XStep() { return m_texCoordY_XStep;}
        float getTexCoordY_YStep() { return m_texCoordY_YStep;}
        float getOneOverZ_XStep() { return m_one_over_z_XStep;}
        float getOneOverZ_YStep() { return m_one_over_z_YStep;}
        float calcXStep(float *values, Vertex minYVert, Vertex midYVert, Vertex maxYVert, float one_over_dx);
        float calcYStep(float *values, Vertex minYVert, Vertex midYVert, Vertex maxYVert, float one_over_dy);
        ~Gradients();

    protected:
};

#endif // GRADIENTS_H
