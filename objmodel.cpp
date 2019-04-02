#include "objmodel.h"

OBJModel::OBJModel(char *file_name)
{
    m_has_normals = false;
    m_has_texCoords = false;
    no_of_triangles = 0;
    cout << file_name << " getting created" << endl;
    ifstream fin;
    fin.open(file_name);
    if(!fin) {
        cout << "OBJ file did not open!!!" << endl;
        has_loaded = false;
        return;
    }
    char type[3];
    while(!fin.eof()) {
        fin >> type;
        //cout << type << endl;
        if(type[0] == 'v' && type[1] == '\0') {
            //cout << "v seen" << endl;
            float position1;
            float position2;
            float position3;
            fin >> position1 >> position2 >> position3;
            //cout << position1 << position2 << position3 << endl;
            addElement(m_positions, Vector4f(position1, position2, position3, 1));
        }
        else if(type[0] == 'v' && type[1] == 't') {
            //cout << "vt seen" << endl;
            float position1;
            float position2;
            fin >> position1 >> position2;
            addElement(m_texCoords, Vector4f(position1, 1.0 - position2, 0, 0));
            m_has_texCoords = true;
        }
        else if(type[0] == 'v' && type[1] == 'n') {
            //cout << "vn seen" << endl;
            float position1;
            float position2;
            float position3;
            fin >> position1 >> position2 >> position3;
            addElement(m_normals, Vector4f(position1, position2, position3, 1));
            m_has_normals = true;
        }
        else if(type[0] == 'f' && type[1] == '\0') {
            //cout << "f seen" << endl;
            int no_components = 1;
            if(m_has_normals == true) {
                no_components++;
            }
            if(m_has_texCoords == true) {
                no_components++;
            }

            //to get face
            int no_of_vertices = 0;
            int start = fin.tellg();
            if(start == -1) {
                break;
            }
            {
                char temp[500];
                fin.get(temp, 500);
                if(temp[0] == '\0') {
                    break;
                }
                for(int i = 0; temp[i]!='\0'; i++) {
                    if(temp[i] == ' ') {
                        no_of_vertices++;
                    }
                }
                fin.seekg(start);
            }
            OBJIndex index[no_of_vertices];

            for(int i = 0; i < no_of_vertices; i++) {
                char info[100];
                fin >> info;
                char **result = getSplit(info, '/');
                addIndices(index[i], result);
                delete result;
                if(no_of_vertices > 3) {
                    addElement(m_indices, index[0]);
                    addElement(m_indices, index[i - 1]);
                }
                addElement(m_indices, index[i]);
            }
            no_of_triangles += no_of_vertices - 2;
        }
        else {
            char temp[100];
            fin.get(temp, 100);
            //cout << temp << endl;
        }
    }
    cout << file_name << " created with " << no_of_triangles << " triangles" << endl;
    has_loaded = true;
}

void OBJModel::addIndices(OBJIndex &index, char **result) {
    for(int i = 0; i < 3; i++) {
        int temp = 0;
        for(int j = 0; result[i][j] != '\0'; j++) {
            temp *= 10;
            temp += (int)result[i][j] - (int)'0';
        }
        //cout << "temp: " << temp << endl;
        if(i == 0) {
            index.setPositionsIndex(temp - 1);
        }
        else if(i == 1) {
            index.setTexCoordsIndex(temp - 1);
        }
        else if(i == 2) {
            index.setNormalsIndex(temp - 1);
        }
    }
}

char** OBJModel::getSplit(char *data, char delimiter) {
    int no_of_sub_strings = 1;
    int max_sub_string = 0;
    int curr_length = 0;
    for(int i = 0; data[i] != '\0'; i++) {
        if(data[i] == delimiter) {
            no_of_sub_strings++;
            if(max_sub_string < curr_length) {
                max_sub_string = curr_length;
            }
            curr_length = 0;
        }
        else {
            curr_length++;
        }
    }
    char **result = new char*[no_of_sub_strings];
    for(int i = 0; i < no_of_sub_strings; i++) {
        result[i] = new char[max_sub_string];
    }
    int line_no = 0;
    int repeater = 0;
    for(int i = 0; data[i] != '\0'; i++) {
        if(data[i] == delimiter || data[i] == '\0') {
            result[line_no][repeater] = '\0';
            line_no++;
            repeater = 0;
        }
        else {
            result[line_no][repeater] = data[i];
            repeater++;
        }
    }
    return result;
}


void OBJModel::addElement(list<Vector4f> &m_list, Vector4f data) {
    list<Vector4f>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

Vector4f OBJModel::getElement(list<Vector4f> &m_list, int index) {
    list<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

void OBJModel::addElement(list<int> &m_list, int data) {
    list<int>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

int OBJModel::getElement(list<int> &m_list, int index) {
    list<int>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

void OBJModel::addElement(list<OBJIndex> &m_list, OBJIndex data) {
    list<OBJIndex>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

OBJModel::OBJIndex OBJModel::getElement(list<OBJIndex> &m_list, int index) {
    list<OBJIndex>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

void OBJModel::deleteElement(list<OBJIndex> &m_list, int index) {
    list<OBJIndex>::iterator it;
    it = m_list.begin();
    advance(it, index);
    m_list.erase(it);
}

void OBJModel::deleteElement(list<OBJIndex> &m_list) {
    list<OBJIndex>::iterator it;
    it = m_list.end();
    m_list.erase(it);
}

void OBJModel::printALL() {
    for(int i = 0; i < m_positions.size(); i++) {
        cout << "Position: ";
        getElement(m_positions, i).printALL();
    }
    for(int i = 0; i < m_texCoords.size(); i++) {
        cout << "texCoords: ";
        getElement(m_texCoords, i).printALL();
    }
    for(int i = 0; i < m_normals.size(); i++) {
        cout << "Normals: ";
        getElement(m_normals, i).printALL();
    }
    for(int i = 0; i < m_indices.size(); i+=3) {
        cout << "Vertex 1: "; getElement(m_indices, i + 0).printALL();
        cout << "Vertex 2: "; getElement(m_indices, i + 1).printALL();
        cout << "Vertex 3: "; getElement(m_indices, i + 2).printALL();
        cout << endl;
    }
}

IndexedModel OBJModel::toIndexedModel() {
    IndexedModel result;

    //cout << "No of Indices: " << m_indices.size() << endl;

    int face_count = 0;
    for(int i = 0; i < m_indices.size(); i++) {
        int m_positions_index = getElement(m_indices, i).getPositionsIndex();
        addElement(result.getPositions(), getElement(m_positions, m_positions_index));
        if(m_has_texCoords == true) {
            int m_texCoords_index = getElement(m_indices, i).getTexCoordsIndex();
            addElement(result.getTexCoords(), getElement(m_texCoords, m_texCoords_index));
        }
        else {
            addElement(result.getTexCoords(), Vector4f(0, 0, 0, 0));
        }
        if(m_has_normals == true) {
            int m_normals_index = getElement(m_indices, i).getNormalsIndex();
            addElement(result.getNormals(), getElement(m_normals, m_normals_index));
        }
        else {
            addElement(result.getNormals(), Vector4f(0, 0, 0, 0));
        }
        addElement(result.getIndices(), i);

//        cout << "Face " << face_count/3 + 1 << ": ";
//        cout << "Vertex " << i%4 << ": ";
//        cout << "Positions Index: " << m_positions_index << " TexCoords Index: " << m_texCoords_index << " Normals Index: " << m_normals_index << endl;
//
//        cout << "Face " << face_count/3 + 1 << ": ";
//        cout << "Vertex " << i%4 << ": " << endl;
//        cout << "Position: "; getElement(m_positions, m_positions_index).printALL();
//        cout << "TexCoord: "; getElement(m_texCoords, m_texCoords_index).printALL();
//        cout << "Normals: "; getElement(m_normals, m_normals_index).printALL();
//        cout << endl;
        face_count++;
    }

    cout << "IndexedModel has been made" << endl;
    return result;
}

OBJModel::~OBJModel()
{
    m_positions.clear();
    m_texCoords.clear();
    m_normals.clear();
    m_indices.clear();
}
