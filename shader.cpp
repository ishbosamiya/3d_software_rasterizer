#include "shader.h"

Vertex Gouraud_Shader::vertexShader(Vertex vertex, int nth_vertex, int width, int height) {
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(width/2.0, height/2.0);
    Vertex result = vertex.transform(screen_space_transform).perspectiveDivide();
    float lighting = std::max(0.0f, vertex.getNormal().normalized().dot(light_dir.normalized()));
    if(nth_vertex == 0) {
        varying_intensity.setX(lighting);
    }
    else if(nth_vertex == 1) {
        varying_intensity.setY(lighting);
    }
    else if(nth_vertex == 2) {
        varying_intensity.setZ(lighting);
    }
}

bool Gouraud_Shader::fragmentShader(Vector4f bar, Colour &colour) {
    float intensity = varying_intensity.dot(bar);
    colour.multiply(intensity);
    return true;
}
