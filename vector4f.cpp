#include "vector4f.h"

Vector4f::Vector4f()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4f::Vector4f(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Vector4f::initialize(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//returns the length of the vector
float Vector4f::length() {
    return (float)sqrt(x*x + y*y + z*z + w*w);
}

//returns the max of the components of the vector
float Vector4f::max() {
    int max = x;
    if(y > max)
        max = y;
    if(z > max)
        max = z;
    if(w > max)
        max = w;
    return max;
}

//returns the dot product of 2 vectors
float Vector4f::dot(Vector4f r) {
    return (x*r.getX() + y*r.getY() + z*r.getZ() + w*r.getW());
}

//returns the cross product of 2 vectors
Vector4f Vector4f::cross(Vector4f r) {
    float m_x = y * r.getZ() - z * r.getY();
    float m_y = z * r.getX() - x * r.getZ();
    float m_z = x * r.getY() - y * r.getX();
    return Vector4f(m_x, m_y, m_z, 0);
}

//makes the length of the vector 1
Vector4f Vector4f::normalized() {
    float len = length();

    return Vector4f(x/len, y/len, z/len, w/len);
}

//rotates the vector based on the axis and the angle
Vector4f Vector4f::rotate(Vector4f axis, float angle) {
    float sin_ = (float)sin(-angle);
    float cos_ = (float)cos(-angle);
    return this->cross(axis.mul(sin_).add(this->mul(cos_).add(axis.mul(this->dot(axis.mul(1 - cos_))))));
}

Vector4f Vector4f::rotate(Quarternion rotation) {
    Quarternion conjugate = rotation.conjugate();

    Quarternion w = rotation.mul(*this).mul(conjugate);

    return Vector4f(w.getX(), w.getY(), w.getZ(), 1.0);
}

//lerps the vector
Vector4f Vector4f::lerp(Vector4f dest, float lerp_factor) {
    return dest.sub(returnThis()).mul(lerp_factor).add(returnThis());
}

//add 2 vectors
Vector4f Vector4f::add(Vector4f r) {
    return Vector4f(x + r.getX(), y + r.getY(), z + r.getZ(), w + r.getW());
}

//adds float value to the vector
Vector4f Vector4f::add(float r) {
    return Vector4f(x + r, y + r, z + r, w + r);
}

//subtracts 2 vectors
Vector4f Vector4f::sub(Vector4f r) {
    return Vector4f(x - r.getX(), y - r.getY(), z - r.getZ(), w - r.getW());
}

//subtracts float value to the vector
Vector4f Vector4f::sub(float r) {
    return Vector4f(x - r, y - r, z - r, w - r);
}

//multiplies 2 vectors
Vector4f Vector4f::mul(Vector4f r) {
    return Vector4f(x * r.getX(), y * r.getY(), z * r.getZ(), w * r.getW());
}

//multiplies float value to the vector
Vector4f Vector4f::mul(float r) {
    return Vector4f(x * r, y * r, z * r, w * r);
}

//divides 2 vectors
Vector4f Vector4f::div(Vector4f r) {
    return Vector4f(x / r.getX(), y / r.getY(), z / r.getZ(), w / r.getW());
}

//divides float value to the vector
Vector4f Vector4f::div(float r) {
    return Vector4f(x / r, y / r, z / r, w / r);
}

//returns vector with all the components having only positive sign
Vector4f Vector4f::abs() {
    return Vector4f(fabs(x), fabs(y), fabs(z), fabs(w));
}

float Vector4f::getX() {
    return x;
}

float Vector4f::getY() {
    return y;
}

float Vector4f::getZ() {
    return z;
}

float Vector4f::getW() {
    return w;
}

bool Vector4f::operator == (Vector4f r) {
    if(x == r.getX() && y == r.getY() && z == r.getZ() && w == r.getW())
        return true;
    else
        return false;
}

Vector4f Vector4f::returnThis() {
    return Vector4f(x, y, z, w);
}

void Vector4f::printALL() {
    cout << "x: " << x << " y: " << y << " z: " << z << " w: " << w << endl;
}

Vector4f::~Vector4f()
{
    //dtor
}
