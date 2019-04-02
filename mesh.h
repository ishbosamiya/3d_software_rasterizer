#ifndef MESH_H
#define MESH_H

#include <list>
#include <iostream>
#include <fstream>
#include <string.h>

#include "indexedmodel.h"
#include "objmodel.h"
#include "vertex.h"

class Mesh
{
    list<Vertex> m_vertices;
    list<int> m_indices;
    bool is_initialized;
    char *m_file_name;

    void addElement(list<Vertex> &m_list, Vertex data);
    void addElement(list<int> &m_list, int data);

    Vector4f getElement(list<Vector4f> &m_list, int index);
    Vertex getElement(list<Vertex> &m_list, int index);
    int getElement(list<int> &m_list, int index);

    bool readMeshFromFile();
    bool writeMeshToFile();

    public:
        Mesh();
        Mesh(char *file_name);

        void initialize(char *file_name);

        Vertex getVertex(int i) { return getElement(m_vertices, i);}
        int getIndex(int i) { return getElement(m_indices, i);}
        int getNumIndices() { return m_indices.size();}
        bool isInitialized() { return is_initialized;}

        virtual ~Mesh();

    protected:
};

#endif // MESH_H
