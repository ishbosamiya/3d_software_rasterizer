#include "quarternion.h"

Quarternion::Quarternion() {
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_w = 0;
}

Quarternion::Quarternion(float x, float y, float z, float w) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

Quarternion::Quarternion(Vector4f axis, float angle) {
    float sin_half_angle = sin(angle/2);
    float cos_half_angle = cos(angle/2);

    m_x = axis.getX() * sin_half_angle;
    m_y = axis.getY() * sin_half_angle;
    m_z = axis.getZ() * sin_half_angle;
    m_w = cos_half_angle;
}

Quarternion::Quarternion(Matrix4f rot) {
    float trace = rot.get(0, 0) + rot.get(1, 1) + rot.get(2, 2);

    if(trace > 0) {
        float s = 0.5/sqrt(trace + 1.0);
        m_w = 0.25/s;
        m_x = (rot.get(1, 2) - rot.get(2, 1)) * s;
        m_y = (rot.get(2, 0) - rot.get(0, 2)) * s;
        m_z = (rot.get(0, 1) - rot.get(1, 0)) * s;
    }
    else {
        if(rot.get(0, 0) > rot.get(1, 1) && rot.get(0, 0) > rot.get(2, 2)) {
            float s = 2.0 * sqrt(1.0 + rot.get(0, 0) - rot.get(1, 1) - rot.get(2, 2));
            m_w = (rot.get(1, 2) - rot.get(2, 1)) / s;
            m_x = 0.25 * s;
            m_y = (rot.get(1, 0) - rot.get(0, 1)) / s;
            m_z = (rot.get(2, 0) - rot.get(0, 2)) / s;
        }
        else if(rot.get(1, 1) > rot.get(2, 2)) {
            float s = 2.0f * sqrt(1.0f + rot.get(1, 1) - rot.get(0, 0) - rot.get(2, 2));
            m_w = (rot.get(2, 0) - rot.get(0, 2)) / s;
            m_x = (rot.get(1, 0) + rot.get(0, 1)) / s;
            m_y = 0.25f * s;
            m_z = (rot.get(2, 1) + rot.get(1, 2)) / s;
        }
        else {
            float s = 2.0f * sqrt(1.0f + rot.get(2, 2) - rot.get(0, 0) - rot.get(1, 1));
            m_w = (rot.get(0, 1) - rot.get(1, 0) ) / s;
            m_x = (rot.get(2, 0) + rot.get(0, 2) ) / s;
            m_y = (rot.get(1, 2) + rot.get(2, 1) ) / s;
            m_z = 0.25f * s;
        }
    }

    float length = sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
    m_x /= length;
    m_y /= length;
    m_z /= length;
    m_w /= length;
}

void Quarternion::initialize(float x, float y, float z, float w) {
    m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

float Quarternion::length() {
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);
}

Quarternion Quarternion::normalized() {
    float length_ = length();
    return Quarternion(m_x/length_, m_y/length_, m_z/length_, m_w/length_);
}

Quarternion Quarternion::conjugate() {
    return Quarternion(-m_x, -m_y, -m_z, m_w);
}

Quarternion Quarternion::mul(float r) {
    return Quarternion(m_x * r, m_y * r, m_z *r, m_w * r);
}

Quarternion Quarternion::mul(Quarternion r) {
    float w = m_w * r.getW() - m_x * r.getX() - m_y *r.getY() - m_z * r.getZ();
    float x = m_x * r.getW() + m_w * r.getX() + m_y *r.getZ() - m_z * r.getY();
    float y = m_y * r.getW() + m_w * r.getY() + m_z *r.getX() - m_x * r.getZ();
    float z = m_z * r.getW() + m_w * r.getZ() + m_x *r.getY() - m_y * r.getX();

    return Quarternion(x, y, z, w);
}

Quarternion Quarternion::mul(Vector4f r) {
    float w = - m_x * r.getX() - m_y *r.getY() - m_z * r.getZ();
    float x = m_w * r.getX() + m_y *r.getZ() - m_z * r.getY();
    float y = m_w * r.getY() + m_z *r.getX() - m_x * r.getZ();
    float z = m_w * r.getZ() + m_x *r.getY() - m_y * r.getX();

    return Quarternion(x, y, z, w);
}

Quarternion Quarternion::sub(Quarternion r) {
    return Quarternion(m_x - r.getX(), m_y - r.getY(), m_z - r.getZ(), m_w - r.getW());
}

Quarternion Quarternion::add(Quarternion r) {
    return Quarternion(m_x + r.getX(), m_y + r.getY(), m_z + r.getZ(), m_w + r.getW());
}

Matrix4f Quarternion::toRotationMatrix() {
    Vector4f forward(2.0f * (m_x * m_z - m_w * m_y), 2.0f * (m_y * m_z + m_w * m_x), 1.0f - 2.0f * (m_x * m_x + m_y * m_y), 1.0);
    Vector4f up(2.0f * (m_x * m_y + m_w * m_z), 1.0f - 2.0f * (m_x * m_x + m_z * m_z), 2.0f * (m_y * m_z - m_w * m_x), 1.0);
    Vector4f right(1.0f - 2.0f * (m_y * m_y + m_z * m_z), 2.0f * (m_x * m_y - m_w * m_z), 2.0f * (m_x * m_z + m_w * m_y), 1.0);

    Matrix4f temp;
    return temp.initRotation(forward, up, right);
}

float Quarternion::dot(Quarternion r) {
    return m_x * r.getX() + m_y * r.getY() + m_z * r.getZ() + m_w * r.getW();
}

Quarternion Quarternion::nLerp(Quarternion dest, float lerp_factor, bool shortest) {
    Quarternion corrected_dest = dest;

    if(shortest && this->dot(dest) < 0) {
        corrected_dest.initialize(-dest.getX(), -dest.getY(), -dest.getZ(), -dest.getW());
    }

    return corrected_dest.sub(*this).mul(lerp_factor).add(*this).normalized();
}

Quarternion Quarternion::sLerp(Quarternion dest, float lerp_factor, bool shortest) {
    const float epsilon = 1e3f;

    float cos = this->dot(dest);
    Quarternion corrected_dest = dest;

    if(shortest && cos < 0) {
        cos = -cos;
        corrected_dest.initialize(-dest.getX(), -dest.getY(), -dest.getZ(), -dest.getW());
    }

    if(fabs(cos) >= 1 - epsilon) {
        return nLerp(corrected_dest, lerp_factor, false);
    }

    float sin_ = sqrt(1.0  - cos*cos);
    float angle = atan2(sin_, cos);
    float inv_sin = 1.0/sin_;

    float src_factor = sin((1.0 - lerp_factor) * angle) * inv_sin;
    float dest_factor = sin((lerp_factor) * angle) * inv_sin;

    return this->mul(src_factor).add(corrected_dest.mul(dest_factor));
}

Vector4f Quarternion::getForward() {
    return Vector4f(0, 0, 1, 1).rotate(*this);
}

Vector4f Quarternion::getBack() {
    return Vector4f(0, 0, -1, 1).rotate(*this);
}

Vector4f Quarternion::getUp() {
    return Vector4f(0, 1, 0, 1).rotate(*this);
}

Vector4f Quarternion::getDown() {
    return Vector4f(0, -1, 0, 1).rotate(*this);
}

Vector4f Quarternion::getLeft() {
    return Vector4f(-1, 0, 0, 1).rotate(*this);
}

Vector4f Quarternion::getRight() {
    return Vector4f(1, 0, 0, 1).rotate(*this);
}

float Quarternion::getX() {
    return m_x;
}

float Quarternion::getY() {
    return m_y;
}

float Quarternion::getZ() {
    return m_z;
}

float Quarternion::getW() {
    return m_w;
}

bool Quarternion::equals(Quarternion r) {
    return (m_x == r.getX() && m_y == r.getY() && m_z == r.getZ() && m_w == r.getW());
}


Quarternion::~Quarternion()
{
    //dtor
}
