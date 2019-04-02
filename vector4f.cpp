#include "vector4f.h"

#include <math.h>
using namespace std;

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

float Vector4f::length() {
    return (float)sqrt(x*x + y*y + z*z + w*w);
}

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

float Vector4f::dot(Vector4f r) {
    return (x*r.getX() + y*r.getY() + z*r.getZ() + w*r.getW());
}

Vector4f Vector4f::cross(Vector4f r) {
    float m_x = y * r.getZ() - z * r.getY();
    float m_y = z * r.getX() - x * r.getZ();
    float m_z = x * r.getY() - y * r.getX();
    return Vector4f(m_x, m_y, m_z, 0);
}

Vector4f Vector4f::normalized() {
    float len = length();

    return Vector4f(x/len, y/len, z/len, w/len);
}

Vector4f Vector4f::rotate(Vector4f axis, float angle) {
    float sin_ = (float)sin(-angle);
    float cos_ = (float)cos(-angle);
    return this->cross(axis.mul(sin_).add(this->mul(cos_).add(axis.mul(this->dot(axis.mul(1 - cos_))))));
}

Vector4f Vector4f::lerp(Vector4f dest, float lerp_factor) {
    return dest.sub(returnThis()).mul(lerp_factor).add(returnThis());
}

Vector4f Vector4f::add(Vector4f r) {
    return Vector4f(x + r.getX(), y + r.getY(), z + r.getZ(), w + r.getW());
}

Vector4f Vector4f::add(float r) {
    return Vector4f(x + r, y + r, z + r, w + r);
}

Vector4f Vector4f::sub(Vector4f r) {
    return Vector4f(x - r.getX(), y - r.getY(), z - r.getZ(), w - r.getW());
}

Vector4f Vector4f::sub(float r) {
    return Vector4f(x - r, y - r, z - r, w - r);
}

Vector4f Vector4f::mul(Vector4f r) {
    return Vector4f(x * r.getX(), y * r.getY(), z * r.getZ(), w * r.getW());
}

Vector4f Vector4f::mul(float r) {
    return Vector4f(x * r, y * r, z * r, w * r);
}

Vector4f Vector4f::div(Vector4f r) {
    return Vector4f(x / r.getX(), y / r.getY(), z / r.getZ(), w / r.getW());
}

Vector4f Vector4f::div(float r) {
    return Vector4f(x / r, y / r, z / r, w / r);
}

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

Vector4f::~Vector4f()
{
    //dtor
}
