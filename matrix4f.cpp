#include "matrix4f.h"
#include <math.h>

//initialization of the matrix
Matrix4f::Matrix4f()
{
    m = new float*[4];
    for(int i = 0; i < 4; i++) {
        m[i] = new float[4];
    }
}

Matrix4f::Matrix4f(const Matrix4f &obj) {
//    std::swap(*m, *obj.m);
//    std::swap(**m, **obj.m);
    m = new float*[4];
    for(int i = 0; i < 4; i++) {
        m[i] = new float[4];
    }
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            m[i][j] = obj.m[i][j];
        }
    }
}

//creating identity matrix
void Matrix4f::initIdentity() {
    m[0][0] = 1;	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = 0;
    m[1][0] = 0;	m[1][1] = 1;	m[1][2] = 0;	m[1][3] = 0;
    m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 1;	m[2][3] = 0;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

//performing a screen space transform
void Matrix4f::initScreenSpaceTransform(float half_width, float half_height) {
    m[0][0] = half_width;	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = half_width;
    m[1][0] = 0;	m[1][1] = -half_height;	m[1][2] = 0;	m[1][3] = half_height;
    m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 1;	m[2][3] = 0;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

//for moving the vertices by x, y and z coordinates
void Matrix4f::initTranslation(float x, float y, float z) {
    m[0][0] = 1;	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = x;
    m[1][0] = 0;	m[1][1] = 1;	m[1][2] = 0;	m[1][3] = y;
    m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 1;	m[2][3] = z;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

void Matrix4f::initRotation(float x, float y, float z, float angle) {
    float sin_ = (float)sin(angle);
    float cos_ = (float)cos(angle);

    m[0][0] = cos_+x*x*(1-cos_);	m[0][1] = x*y*(1-cos_)-z*sin_;	m[0][2] = x*z*(1-cos_)+y*sin_;	m[0][3] = 0;
    m[1][0] = y*x*(1-cos_)+z*sin_;	m[1][1] = cos_+y*y*(1-cos_);	m[1][2] = y*z*(1-cos_)-x*sin_;	m[1][3] = 0;
    m[2][0] = z*x*(1-cos_)-y*sin_;	m[2][1] = z*y*(1-cos_)+x*sin_;	m[2][2] = cos_+z*z*(1-cos_);	m[2][3] = 0;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

void Matrix4f::initRotation(float x, float y, float z) {
    Matrix4f rx;
    Matrix4f ry;
    Matrix4f rz;

    rz.m[0][0] = (float)cos(z);     rz.m[0][1] = -(float)sin(z);    rz.m[0][2] = 0;				    rz.m[0][3] = 0;
    rz.m[1][0] = (float)sin(z);     rz.m[1][1] = (float)cos(z);     rz.m[1][2] = 0;					rz.m[1][3] = 0;
    rz.m[2][0] = 0;					rz.m[2][1] = 0;					rz.m[2][2] = 1;					rz.m[2][3] = 0;
    rz.m[3][0] = 0;					rz.m[3][1] = 0;					rz.m[3][2] = 0;					rz.m[3][3] = 1;

    rx.m[0][0] = 1;					rx.m[0][1] = 0;					rx.m[0][2] = 0;					rx.m[0][3] = 0;
    rx.m[1][0] = 0;					rx.m[1][1] = (float)cos(x);     rx.m[1][2] = -(float)sin(x);    rx.m[1][3] = 0;
    rx.m[2][0] = 0;					rx.m[2][1] = (float)sin(x);     rx.m[2][2] = (float)cos(x);     rx.m[2][3] = 0;
    rx.m[3][0] = 0;					rx.m[3][1] = 0;					rx.m[3][2] = 0;					rx.m[3][3] = 1;

    ry.m[0][0] = (float)cos(y);     ry.m[0][1] = 0;					ry.m[0][2] = -(float)sin(y);    ry.m[0][3] = 0;
    ry.m[1][0] = 0;					ry.m[1][1] = 1;					ry.m[1][2] = 0;					ry.m[1][3] = 0;
    ry.m[2][0] = (float)sin(y);     ry.m[2][1] = 0;					ry.m[2][2] = (float)cos(y);     ry.m[2][3] = 0;
    ry.m[3][0] = 0;					ry.m[3][1] = 0;					ry.m[3][2] = 0;					ry.m[3][3] = 1;

    Matrix4f temp = rz.mul(ry.mul(rx));

    float **temp_matrix = temp.getM();
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            this->m[i][j] = temp_matrix[i][j];
        }
    }
}

void Matrix4f::initScale(float x, float y, float z) {
    m[0][0] = x;	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = 0;
    m[1][0] = 0;	m[1][1] = y;	m[1][2] = 0;	m[1][3] = 0;
    m[2][0] = 0;	m[2][1] = 0;	m[2][2] = z;	m[2][3] = 0;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

void Matrix4f::initPerspective(float fov, float aspect_ratio, float z_near, float z_far) {
    float tan_half_FOV = (float)tan(fov/2.0);
    float z_range = z_near - z_far;

    m[0][0] = 1.0/(tan_half_FOV * aspect_ratio);	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = 0;
    m[1][0] = 0;	m[1][1] = 1.0/tan_half_FOV;	m[1][2] = 0;	m[1][3] = 0;
    m[2][0] = 0;	m[2][1] = 0;	m[2][2] = (-z_near - z_far)/z_range;	m[2][3] = 2 * z_far * z_near / z_range;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 1;	m[3][3] = 0;
}

void Matrix4f::initOrthographic(float left, float right, float bottom, float top, float near, float far) {
    float width = right - left;
    float height = top - bottom;
    float depth = far - near;

    m[0][0] = 2.0/width;	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = -(right + left)/width;
    m[1][0] = 0;	m[1][1] = 2.0/height;	m[1][2] = 0;	m[1][3] = -(top + bottom)/height;
    m[2][0] = 0;	m[2][1] = 0;	m[2][2] = -2.0/depth;	m[2][3] = -(far + near)/depth;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

Matrix4f Matrix4f::initRotation(Vector4f forward, Vector4f up) {
    Vector4f f = forward.normalized();
    Vector4f r = up.normalized();
    r = r.cross(f);
    Vector4f u = f.cross(r);

    return initRotation(f, u, r);
}

Matrix4f Matrix4f::initRotation(Vector4f forward, Vector4f up, Vector4f right) {
    Vector4f f = forward;
    Vector4f r = right;
    Vector4f u = up;

    m[0][0] = r.getX();	m[0][1] = r.getY();	m[0][2] = r.getZ();	m[0][3] = 0;
    m[1][0] = u.getX();	m[1][1] = u.getY();	m[1][2] = u.getZ();	m[1][3] = 0;
    m[2][0] = f.getX();	m[2][1] = f.getY();	m[2][2] = f.getZ();	m[2][3] = 0;
    m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

Vector4f Matrix4f::transform(Vector4f r) {
    return Vector4f(m[0][0] * r.getX() + m[0][1] * r.getY() + m[0][2] * r.getZ() + m[0][3] * r.getW(),
                    m[1][0] * r.getX() + m[1][1] * r.getY() + m[1][2] * r.getZ() + m[1][3] * r.getW(),
                    m[2][0] * r.getX() + m[2][1] * r.getY() + m[2][2] * r.getZ() + m[2][3] * r.getW(),
                    m[3][0] * r.getX() + m[3][1] * r.getY() + m[3][2] * r.getZ() + m[3][3] * r.getW());
}

Matrix4f Matrix4f::mul(Matrix4f r) {
    Matrix4f res;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            res.set(i, j, m[i][0] * r.get(0, j) + m[i][1] * r.get(1, j) + m[i][2] * r.get(2, j) + m[i][3] * r.get(3, j));
        }
    }
    return res;
}

float** Matrix4f::getM() {
    return m;
}

float Matrix4f::get(int x, int y) {
    return m[x][y];
}

void Matrix4f::setM(float **m) {
    this->m = m;
}

void Matrix4f::set(int x, int y, float value) {
    m[x][y] = value;
}

Matrix4f::~Matrix4f()
{
    for(int i = 0; i < 4; i++) {
        delete [] m[i];
    }
    delete [] m;
}
