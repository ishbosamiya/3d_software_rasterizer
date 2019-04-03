#include "shader.h"

Vector4f Shader::light_dir(cos(0), cos(0), cos(0));

Vertex Shader::vertexShader(Vertex vertex, int nth_vertex, int width, int height) {
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(width/2.0, height/2.0);
    Vertex result = vertex.transform(screen_space_transform).perspectiveDivide();
    //float temp = vertex.getNormal().normalized().dot(light_dir.normalized());

    float lighting = vertex.getNormal().dot(light_dir);
    //lighting = fabs(lighting);
    if(nth_vertex == 0) {
        normals[0] = vertex.getNormal();
        varying_intensity.setX(lighting);
    }
    else if(nth_vertex == 1) {
        normals[1] = vertex.getNormal();
        varying_intensity.setY(lighting);
    }
    else if(nth_vertex == 2) {
        normals[2] = vertex.getNormal();
        varying_intensity.setZ(lighting);
    }
    return result;
}

bool Shader::fragmentShader(Vector4f bar, Colour &colour) {
    float intensity = varying_intensity.normalized().dot(bar.normalized());
//    colour.fill(max(intensity * 255.0f, 0.1f * 255.0f));

//    colour.fill(intensity * 255.0f);

    colour.multiply(max((float)(min(intensity * 1.2, 1.0)), (float)(0.1)));

//    colour.fill(255);
//    colour.multiply(normals[0].cross(normals[1].cross(normals[2])).normalized());

//    colour.fill(255);
//    colour.multiply(normals[0]);

//    colour.fill(varying_intensity.normalized().getX() * 255.0);

    return true;
}

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
    return result;
}

bool Gouraud_Shader::fragmentShader(Vector4f bar, Colour &colour) {
    float intensity = varying_intensity.normalized().dot(bar.normalized());
    //cout << intensity << endl;
    colour.fill(255 * intensity);
    //colour.multiply(intensity);
    return false;
}
