#include "indexedmodel.h"

IndexedModel::IndexedModel() {

}

Vector4f IndexedModel::getElement(list<Vector4f> &m_list, int index) {
    list<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

int IndexedModel::getElement(list<int> &m_list, int index) {
    list<int>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

void IndexedModel::setElement(list<Vector4f> &m_list, int index, Vector4f data) {
    list<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    *it = data;
}

void IndexedModel::setElement(list<int> &m_list, int index, int data) {
    list<int>::iterator it;
    it = m_list.begin();
    advance(it, index);
    *it = data;
}

void IndexedModel::calcNormals() {
    for(int i = 0; i < m_indices.size(); i+=3) {
        int i0 = getElement(m_indices, i);
        int i1 = getElement(m_indices, i + 1);
        int i2 = getElement(m_indices, i + 2);

        Vector4f v1 = getElement(m_positions, i1).sub(getElement(m_positions, i0));
        Vector4f v2 = getElement(m_positions, i2).sub(getElement(m_positions, i0));

        Vector4f normal = v1.cross(v2).normalized();

        setElement(m_normals, i1, getElement(m_normals, i0).add(normal));
        setElement(m_normals, i1, getElement(m_normals, i1).add(normal));
        setElement(m_normals, i1, getElement(m_normals, i2).add(normal));
    }
    for(int i = 0; i < m_normals.size(); i++) {
        setElement(m_normals, i, getElement(m_normals, i).normalized());
    }
}

void IndexedModel::calcTangents() {
    for(int i = 0; i < m_indices.size(); i+=3) {
        int i0 = getElement(m_indices, i);
        int i1 = getElement(m_indices, i + 1);
        int i2 = getElement(m_indices, i + 2);

        Vector4f edge1 = getElement(m_positions, i1).sub(getElement(m_positions, i0));
        Vector4f edge2 = getElement(m_positions, i2).sub(getElement(m_positions, i0));

        float delta_u1 = getElement(m_texCoords, i1).getX() - getElement(m_texCoords, i0).getX();
        float delta_v1 = getElement(m_texCoords, i1).getY() - getElement(m_texCoords, i0).getY();
        float delta_u2 = getElement(m_texCoords, i2).getX() - getElement(m_texCoords, i0).getX();
        float delta_v2 = getElement(m_texCoords, i2).getY() - getElement(m_texCoords, i0).getY();

        float dividend = (delta_u1*delta_v2 - delta_u2*delta_v1);
        float f;
        if(dividend == 0.0) {
            f = 0.0;
        }
        else {
            1.0/dividend;
        }

        Vector4f tangent(f*(delta_v2*edge1.getX() - delta_v1*edge2.getX()),
                         f*(delta_v2*edge1.getY() - delta_v1*edge2.getY()),
                         f*(delta_v2*edge1.getZ() - delta_v1*edge2.getZ()),
                         0);

        setElement(m_tangents, i0, getElement(m_tangents, i0).add(tangent));
        setElement(m_tangents, i1, getElement(m_tangents, i1).add(tangent));
        setElement(m_tangents, i2, getElement(m_tangents, i2).add(tangent));
    }

    for(int i = 0; i < m_tangents.size(); i++) {
        setElement(m_tangents, i, getElement(m_tangents, i).normalized());
    }
}

IndexedModel::~IndexedModel()
{
    m_positions.clear();
    m_texCoords.clear();
    m_normals.clear();
    m_tangents.clear();
    m_indices.clear();
}
