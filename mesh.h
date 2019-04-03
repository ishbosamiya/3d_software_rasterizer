#ifndef MESH_H
#define MESH_H

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

#include "indexedmodel.h"
#include "objmodel.h"
#include "face.h"
#include "vertex.h"

class Mesh
{
    int no_of_positions;
    int no_of_texCoords;
    int no_of_normals_smooth;
    int no_of_faces;
    vector<Vector4f> m_positions;
    vector<Vector4f> m_texCoords;
    vector<Vector4f> m_normals_smooth;
    vector<Face> m_faces;
    bool is_initialized;
    char *m_file_name;

    template <typename T>
    void addElement(vector<T> &m_list, T data);
    template <typename T>
    T getElement(list<T> &m_list, int index);
    template <typename T>
    void equateListToVector(vector<T> &m_vector, list<T> &m_list);
    template <typename T>
    T getElement(vector<T> &m_list, int index);

    void calculateSmoothNormals();
    void calculateFlatNormals();

    bool readMeshFromFile();
    bool writeMeshToFile();

    public:
        Mesh();
        Mesh(char *file_name);

        Mesh* initialize(char *file_name);

        int getNumOfFaces() { return m_faces.size();}
        Face getFace(int loc) { return m_faces[loc];}
        Vector4f getPosition(int loc) { return m_positions[loc];}
        Vector4f getTexCoord(int loc) { return m_texCoords[loc];}
        Vector4f getNormalSmooth(int loc) { return m_normals_smooth[loc];}
        vector<Face> &getFaces() { return m_faces;}
        vector<Vector4f> &getPositions() { return m_positions;}
        vector<Vector4f> &getTexCoords() { return m_texCoords;}
        vector<Vector4f> &getNormalsSmooth() { return m_normals_smooth;}
        bool isInitialized() { return is_initialized;}

        virtual ~Mesh();

    protected:
};

#endif // MESH_H
