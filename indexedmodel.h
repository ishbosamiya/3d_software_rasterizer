#ifndef INDEXEDMODEL_H
#define INDEXEDMODEL_H

#include <list>

#include "vector4f.h"

using namespace std;

class IndexedModel
{
    list<Vector4f> m_positions;
    list<Vector4f> m_texCoords;
    list<Vector4f> m_normals;
    list<Vector4f> m_tangents;
    list<int> m_indices;

    public:
        IndexedModel();

        void calcNormals();
        void calcTangents();

        Vector4f getElement(list<Vector4f> &m_list, int index);
        int getElement(list<int> &m_list, int index);
        void setElement(list<Vector4f> &m_list, int index, Vector4f data);
        void setElement(list<int> &m_list, int index, int data);

        list<Vector4f>& getPositions() { return m_positions;}
        list<Vector4f>& getTexCoords() { return m_texCoords;}
        list<Vector4f>& getNormals() { return m_normals;}
        list<Vector4f>& getTangents() { return m_tangents;}
        list<int>& getIndices() { return m_indices;}


        virtual ~IndexedModel();

    protected:
};

#endif // INDEXEDMODEL_H
