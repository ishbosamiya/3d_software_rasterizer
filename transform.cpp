#include "transform.h"

Transform::Transform() {
    *this = Transform(Vector4f(0, 0, 0, 0));
}

Transform::Transform(Vector4f pos) {
    *this = Transform(pos, Quarternion(0, 0, 0, 1), Vector4f(1, 1, 1, 1));
}

Transform::Transform(Vector4f pos, Quarternion rot, Vector4f scale) {
    m_pos = pos;
    m_rot = rot;
    m_scale = scale;
}

Transform Transform::setPos(Vector4f pos) {
    return Transform(pos, m_rot, m_scale);
}

Transform Transform::rotate(Quarternion rotation) {
    return Transform(m_pos, rotation.mul(m_rot).normalized(), m_scale);
}

Transform Transform::lookAt(Vector4f point, Vector4f up) {
    return rotate(getLookAtRotation(point, up));
}

Quarternion Transform::getLookAtRotation(Vector4f point, Vector4f up) {
    Matrix4f temp;
    return Quarternion(temp.initRotation(point.sub(m_pos).normalized(), up));
}

Matrix4f Transform::getTransformation() {
    Matrix4f translation_matrix;
    translation_matrix.initTranslation(m_pos.getX(), m_pos.getY(), m_pos.getZ());
    Matrix4f rotation_matrix = m_rot.toRotationMatrix();
    Matrix4f scale_matrix;
    scale_matrix.initScale(m_scale.getX(), m_scale.getY(), m_scale.getZ());

    return translation_matrix.mul(rotation_matrix.mul(scale_matrix));
}

Vector4f Transform::getTransformedPos() {
    return m_pos;
}

Quarternion Transform::getTransformedRot() {
    return m_rot;
}

Vector4f Transform::getPos() {
    return m_pos;
}

Quarternion Transform::getRot() {
    return m_rot;
}

Vector4f Transform::getScale() {
    return m_scale;
}

Transform::~Transform()
{
    //dtor
}
