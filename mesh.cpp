#include "mesh.h"

Mesh::Mesh() {
    is_initialized = false;
}

Mesh::Mesh(char *file_name)
{
    initialize(file_name);
}

Mesh* Mesh::initialize(char *file_name) {
    if(is_initialized) {
        return this;
    }
//    *this = Mesh(file_name);
    m_file_name = file_name;
    char *temp = new char[500];
    strcpy(temp, file_name);
    strcat(temp, ".mesh");
    ifstream fin;
    fin.open(temp);

    if(!fin.is_open()) {
        OBJModel obj(file_name);
        if(obj.hasLoaded() == false) {
            cout << "Failed to find obj of " << file_name << endl;
            is_initialized = false;
            return this;
        }
        IndexedModel model;
        obj.toIndexedModel(model);

        for(int i = 0; i < model.getPositions().size(); i++) {
            addElement(m_vertices, Vertex(getElement(model.getPositions(), i), getElement(model.getTexCoords(), i)));
        }

        equateListToVector(m_indices, model.getIndices());
        writeMeshToFile();
        is_initialized = true;
    }
    else {
        if(!readMeshFromFile()) {
            OBJModel obj(file_name);
            if(obj.hasLoaded() == false) {
                cout << "Failed to find obj of " << file_name << endl;
                is_initialized = false;
                return this;
            }
            IndexedModel model;
            obj.toIndexedModel(model);

            for(int i = 0; i < model.getPositions().size(); i++) {
                addElement(m_vertices, Vertex(getElement(model.getPositions(), i), getElement(model.getTexCoords(), i)));
            }

            equateListToVector(m_indices, model.getIndices());
            writeMeshToFile();
            is_initialized = true;
        }
    }
    cout << file_name << " has been loaded" << endl;
    fin.close();
    delete temp;
    return this;
}

bool Mesh::writeMeshToFile() {
    char *temp = new char[500];
    strcpy(temp, m_file_name);
    strcat(temp, ".mesh");
    ofstream fout;
    fout.open(temp, ios::binary);
    if(!fout.is_open()) {
        cout << "Error writing to file" << endl;
        delete temp;
        return false;
    }

    for(int i = 0; i < getNumIndices(); i++) {
        Vertex temp_vertex = getVertex(getIndex(i));
        int temp_int = getIndex(i);
        fout.write((char *)&temp_vertex, sizeof(Vertex));
        fout.write((char *)&temp_int, sizeof(int));
    }
    cout << m_file_name << " has been written to file" << endl;
    fout.close();
    delete temp;
    return true;
}

bool Mesh::readMeshFromFile() {
    char *temp = new char[500];
    strcpy(temp, m_file_name);
    strcat(temp, ".mesh");
    ifstream fin;
    fin.open(temp, ios::binary);
    if(!fin.is_open()) {
        cout << "Error reading mesh from stored file" << endl;
        return false;
    }
    bool index_line = false;
    while(!fin.eof()) {
        if(index_line == false) {
            Vertex temp_vertex;
            fin.read((char *)&temp_vertex, sizeof(Vertex));
            addElement(m_vertices, temp_vertex);
            index_line = !index_line;
        }
        else {
            int temp_int;
            fin.read((char *)&temp_int, sizeof(int));
            addElement(m_indices, temp_int);
            index_line = !index_line;
        }
    }
    cout << m_file_name << " has been read from file" << endl;
    is_initialized = true;
    fin.close();
    delete temp;
    return true;
}

void Mesh::addElement(vector<Vertex> &m_list, Vertex data) {
    vector<Vertex>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

void Mesh::addElement(vector<int> &m_list, int data) {
    vector<int>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

Vector4f Mesh::getElement(vector<Vector4f> &m_list, int index) {
    vector<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

Vector4f Mesh::getElement(list<Vector4f> &m_list, int index) {
    list<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

Vertex Mesh::getElement(vector<Vertex> &m_list, int index) {
    vector<Vertex>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

int Mesh::getElement(vector<int> &m_list, int index) {
    vector<int>::iterator it;
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

void Mesh::equateListToVector(vector<int> &m_vector, list<int> &m_list) {
    for(int i = 0; i < m_list.size(); i++) {
        int data = getElement(m_list, i);
        m_vector.push_back(data);
    }
}

Mesh::~Mesh()
{
    m_vertices.clear();
    m_indices.clear();
    delete [] m_file_name;
}
