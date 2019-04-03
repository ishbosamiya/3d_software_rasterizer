#include "camera.h"

Camera::Camera(Matrix4f projection, int width, int height) {
    Y_AXIS.initialize(0, 1, 0, 1);
    m_projection = projection;
    m_width = width;
    m_height = height;
}

Matrix4f Camera::getViewProjection() {
    Matrix4f camera_rotation = getTransform().getTransformedRot().conjugate().toRotationMatrix();
    Vector4f camera_pos = getTransform().getTransformedPos().mul(-1.0);

    Matrix4f camera_translation;
    camera_translation.initTranslation(camera_pos.getX(), camera_pos.getY(), camera_pos.getZ());

    return m_projection.mul(camera_rotation.mul(camera_translation));
}

//to check
void Camera::update(Input &input, float delta, int x, int y, float mov_amt) {
    static int prev_x = x;
    static int prev_y = y;
    float sensitivity_x = delta * 0.0001 * (abs(prev_x - x)) * 0.5;
    if(prev_x <= 0 || prev_x >= m_width || x <= 0 || y >= m_width) {
        sensitivity_x = 0;
    }
    float sensitivity_y = delta * 0.00005 * (abs(prev_y - y)) * 0.5;
    if(prev_y <= 0 || prev_y >= m_height || y <= 0 || y >= m_height) {
        sensitivity_y = 0;
    }

    if(x < prev_x) {
        rotate(Y_AXIS, -sensitivity_x);
        prev_x = x;
    }
    else if(x > prev_x) {
        rotate(Y_AXIS, sensitivity_x);
        prev_x = x;
    }
    if(y < prev_y) {
        rotate(getTransform().getRot().getRight(), -sensitivity_y);
        prev_y = y;
    }
    else if(y > prev_y) {
        rotate(getTransform().getRot().getRight(), sensitivity_y);
        prev_y = y;
    }

    //keyboard
    if(input.isPressed(KEY_W)) {
        move(getTransform().getRot().getForward(), mov_amt);
    }
    if(input.isPressed(KEY_S)) {
        move(getTransform().getRot().getForward(), -mov_amt);
    }
    if(input.isPressed(KEY_A)) {
        move(getTransform().getRot().getLeft(), mov_amt);
    }
    if(input.isPressed(KEY_D)) {
        move(getTransform().getRot().getRight(), mov_amt);
    }
}

void Camera::move(Vector4f dir, float amt) {
    m_transform = getTransform().setPos(getTransform().getPos().add(dir.mul(amt)));
}

void Camera::rotate(Vector4f axis, float angle) {
    m_transform = getTransform().rotate(Quarternion(axis, angle));
}

Camera::~Camera() {
    //dtor
}
