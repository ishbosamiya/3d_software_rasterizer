#include "mesh.h"

Mesh::Mesh() {
    is_initialized = false;
}

Mesh::Mesh(char *file_name)
{
    IndexedModel model = OBJModel(file_name).toIndexedModel();

    for(int i = 0; i < model.getPositions().size(); i++) {
        addElement(m_vertices, Vertex(getElement(model.getPositions(), i), getElement(model.getTexCoords(), i)));
    }

    m_indices = model.getIndices();
    is_initialized = true;
}

void Mesh::initialize(char *file_name) {
    IndexedModel model = OBJModel(file_name).toIndexedModel();

    for(int i = 0; i < model.getPositions().size(); i++) {
        addElement(m_vertices, Vertex(getElement(model.getPositions(), i), getElement(model.getTexCoords(), i)));
    }

    m_indices = model.getIndices();
    is_initialized = true;
}

void Mesh::addElement(list<Vertex> &m_list, Vertex data) {
    list<Vertex>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

Vector4f Mesh::getElement(list<Vector4f> &m_list, int index) {
    list<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

Vertex Mesh::getElement(list<Vertex> &m_list, int index) {
    list<Vertex>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

int Mesh::getElement(list<int> &m_list, int index) {
    list<int>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

Mesh::~Mesh()
{
    //dtor
}
