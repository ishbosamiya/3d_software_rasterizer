#include "camera.h"

Camera::Camera(Matrix4f projection) {
    Y_AXIS.initialize(0, 1, 0, 1);
    m_projection = projection;
}

Matrix4f Camera::getViewProjection() {
    Matrix4f camera_rotation = getTransform().getTransformedRot().conjugate().toRotationMatrix();
    Vector4f camera_pos = getTransform().getTransformedPos().mul(-1.0);

    Matrix4f camera_translation;
    camera_translation.initTranslation(camera_pos.getX(), camera_pos.getY(), camera_pos.getZ());

    return m_projection.mul(camera_rotation.mul(camera_translation));
}

//to check
void Camera::update(SDL_Event &event, float delta, int x, int y) {
    static int prev_x = x;
    static int prev_y = y;
    const float sensitivity_x = 2.66 * delta * 0.0001;
    const float sensitivity_y = 2.0 * delta * 0.0001;
    const float mov_amt = 5.0 * delta * 0.0005;

    if(x < prev_x) {
        cout << "left" << endl;
        rotate(Y_AXIS, -sensitivity_x);
        prev_x = x;
    }
    else if(x > prev_x) {
        cout << "right" << endl;
        rotate(Y_AXIS, sensitivity_x);
        prev_x = x;
    }
    if(y < prev_y) {
        cout << "up" << endl;
        rotate(getTransform().getRot().getRight(), -sensitivity_y);
        prev_y = y;
    }
    else if(y > prev_y) {
        cout << "down" << endl;
        rotate(getTransform().getRot().getRight(), sensitivity_y);
        prev_y = y;
    }

    switch(event.type) {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    SDL_Quit();
                    return;
                case SDLK_w:
                    cout << "w pressed" << endl;
                    move(getTransform().getRot().getForward(), mov_amt);
                    break;
                case SDLK_s:
                    cout << "s pressed" << endl;
                    move(getTransform().getRot().getForward(), -mov_amt);
                    break;
                case SDLK_a:
                    cout << "a pressed" << endl;
                    move(getTransform().getRot().getLeft(), mov_amt);
                    break;
                case SDLK_d:
                    cout << "d pressed" << endl;
                    move(getTransform().getRot().getRight(), mov_amt);
                    break;
            }
            break;
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
