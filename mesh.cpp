#include "mesh.h"

Mesh::Mesh() {
    is_initialized = false;
    m_file_name = NULL;
}

Mesh::Mesh(char *file_name)
{
    m_file_name = file_name;
    initialize(file_name);
}

Mesh* Mesh::initialize(char *file_name) {
    if(is_initialized) {
        cout << file_name << " has been already been loaded loaded with " << no_of_faces << " no of faces" << endl;
        return this;
    }
    m_file_name = file_name;
    char *temp = new char[strlen(file_name) + 50];
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
        equateListToVector(m_positions, model.getPositions());
        equateListToVector(m_texCoords, model.getTexCoords());
        equateListToVector(m_normals_smooth, model.getNormals());
        equateListToVector(m_faces, model.getFaces());
        no_of_positions = m_positions.size();
        no_of_texCoords = m_texCoords.size();
        no_of_normals_smooth = m_normals_smooth.size();
        no_of_faces = m_faces.size();
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
            equateListToVector(m_positions, model.getPositions());
            equateListToVector(m_texCoords, model.getTexCoords());
            equateListToVector(m_normals_smooth, model.getNormals());
            equateListToVector(m_faces, model.getFaces());
            no_of_positions = m_positions.size();
            no_of_texCoords = m_texCoords.size();
            no_of_normals_smooth = m_normals_smooth.size();
            no_of_faces = m_faces.size();
            writeMeshToFile();
            is_initialized = true;
        }
    }
    cout << file_name << " has been loaded with " << no_of_faces << " no of faces" << endl;
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

    fout.write((char *)&no_of_positions, sizeof(no_of_positions));
    fout.write((char *)&no_of_texCoords, sizeof(no_of_texCoords));
    fout.write((char *)&no_of_normals_smooth, sizeof(no_of_normals_smooth));
    fout.write((char *)&no_of_faces, sizeof(no_of_faces));
    for(int i = 0; i < no_of_positions; i++) {
        Vector4f position = m_positions[i];
        fout.write((char *)&position, sizeof(position));
    }
    for(int i = 0; i < no_of_texCoords; i++) {
        Vector4f texCoord = m_texCoords[i];
        fout.write((char *)&texCoord, sizeof(texCoord));
    }
    for(int i = 0; i < no_of_normals_smooth; i++) {
        Vector4f normal = m_normals_smooth[i];
        fout.write((char *)&normal, sizeof(normal));
    }
    for(int i = 0; i < no_of_faces; i++) {
        Face face = m_faces[i];
        face.writeFaceToFile(fout);
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

    fin.read((char *)&no_of_positions, sizeof(int));
    fin.read((char *)&no_of_texCoords, sizeof(int));
    fin.read((char *)&no_of_normals_smooth, sizeof(int));
    fin.read((char *)&no_of_faces, sizeof(int));
    m_positions.reserve(no_of_positions);
    m_texCoords.reserve(no_of_texCoords);
    m_normals_smooth.reserve(no_of_normals_smooth);
    m_faces.reserve(no_of_faces);
    for(int i = 0; i < no_of_positions; i++) {
        Vector4f position;
        fin.read((char *)&position, sizeof(Vector4f));
        m_positions.push_back(position);
    }
    for(int i = 0; i < no_of_texCoords; i++) {
        Vector4f texCoord;
        fin.read((char *)&texCoord, sizeof(Vector4f));
        m_texCoords.push_back(texCoord);
    }
    for(int i = 0; i < no_of_normals_smooth; i++) {
        Vector4f normal;
        fin.read((char *)&normal, sizeof(Vector4f));
        m_normals_smooth.push_back(normal);
    }
    for(int i = 0; i < no_of_faces; i++) {
        Face face;
        face.readFaceFromFile(fin);
        m_faces.push_back(face);
    }

    cout << m_file_name << " has been read from file" << endl;
    is_initialized = true;
    fin.close();
    delete temp;

    //calculateSmoothNormals();

    return true;
}

void Mesh::calculateFlatNormals() {

}

void Mesh::calculateSmoothNormals() {
    vector<Vector4f> temp_normals;
    temp_normals.reserve(no_of_positions);
    for(int i = 0; i < m_positions.size(); i++) {
        temp_normals.push_back(Vector4f(0, 0, 0, 0));
    }

    for(int i = 0; i < no_of_faces; i++) {
        for(int j = 0; j < m_faces[i].getNumOfVerts(); j++) {
            temp_normals[m_faces[i].getPosition(j)] = temp_normals[m_faces[i].getPosition(j)].add(m_positions[m_faces[i].getPosition(j)]);
            m_faces[i].addNormal(j, m_faces[i].getPosition(j));
        }
    }

    for(int i = 0; i < getNumOfFaces(); i++) {
        vector<Vector4f> positions;
        vector<int> positions_index;
        int no_of_verts = getFace(i).getNumOfVerts();
        positions.reserve(no_of_verts);
        positions_index.reserve(no_of_verts);
        for(int j = 0; j < no_of_verts; j++) {
            Vector4f position = getPosition(getFace(i).getPosition(j));
            positions.push_back(position);
            positions_index.push_back(getFace(i).getPosition(j));
        }
        for(int j = 0; j < positions.size() - 2; j++) {
            Vector4f p1 = positions[j + 1].sub(positions[0]);
            Vector4f p2 = positions[j + 2].sub(positions[0]);
            Vector4f n = p1.cross(p2);
            temp_normals[positions_index[0]] = temp_normals[positions_index[0]].add(n);
            temp_normals[positions_index[j + 1]] = temp_normals[positions_index[j + 1]].add(n);
            temp_normals[positions_index[j + 2]] = temp_normals[positions_index[j + 2]].add(n);
        }
        positions.clear();
        getFace(i).setNormals(positions_index);
        positions_index.clear();
    }

    for(int i = 0; i < temp_normals.size(); i++) {
        temp_normals[i] = temp_normals[i].normalized();
    }

    m_normals_smooth = temp_normals;
    temp_normals.clear();
}

template <typename T>
void Mesh::addElement(vector<T> &m_list, T data) {
    m_list.push_back(data);
}

template <typename T>
T Mesh::getElement(vector<T> &m_list, int index) {
    m_list[index];
}

template <typename T>
T Mesh::getElement(list<T> &m_list, int index) {
    typename list<T>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

template <typename T>
void Mesh::equateListToVector(vector<T> &m_vector, list<T> &m_list) {
    for(int i = 0; i < m_list.size(); i++) {
        T data = getElement(m_list, i);
        m_vector.push_back(data);
    }
}



Mesh::~Mesh()
{
    m_positions.clear();
    m_texCoords.clear();
    m_faces.clear();
    delete [] m_file_name;
}
