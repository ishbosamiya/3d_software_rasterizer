#include "shader.h"

Vector4f Shader::light_dir(cos(0), cos(0), cos(0));
int Shader::shader_type = 1;
int Shader::no_of_shaders = 5;
float Shader::ambient_factor = 0.05;
Colour Shader::base_colour;

Vertex Shader::vertexShader(Vertex vertex, int nth_vertex, int width, int height) {
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(width/2.0, height/2.0);
    Vertex result = vertex.transform(screen_space_transform).perspectiveDivide();

    if(shader_type > -1) {
        float lighting = vertex.getNormalSmooth().normalized().dot(light_dir.normalized());
        if(nth_vertex == 0) {
            normals[0] = vertex.getNormalSmooth();
            varying_intensity.setX(lighting);
        }
        else if(nth_vertex == 1) {
            normals[1] = vertex.getNormalSmooth();
            varying_intensity.setY(lighting);
        }
        else if(nth_vertex == 2) {
            normals[2] = vertex.getNormalSmooth();
            varying_intensity.setZ(lighting);
        }
    }
    return result;
}

bool Shader::fragmentShader(Vector4f bar, Colour &colour) {
    //Unlit
    if(shader_type == -1) {
        return true;
    }

    //Texture + Lighting Smooth
    if(shader_type % no_of_shaders == 0) {
        float intensity = varying_intensity.normalized().dot(bar.normalized());
        intensity = addAmbientFactor(intensity, ambient_factor);
        colour.multiply(intensity);
    }
    //Lighting Smooth Normals
    else if(shader_type % no_of_shaders == 1) {
        colour = base_colour;
        float intensity = varying_intensity.normalized().dot(bar.normalized());
        intensity = addAmbientFactor(intensity, ambient_factor);
        colour.multiply(intensity);
    }
    //Lighting Flat Normals
    else if(shader_type % no_of_shaders == 2) {
        colour = base_colour;
        float intensity = varying_intensity.getX() + varying_intensity.getY() + varying_intensity.getZ();
        intensity /= 3.0;
        intensity = addAmbientFactor(intensity, ambient_factor);
        colour.multiply(intensity);
    }
    //Texture + Lighting Flat
    else if(shader_type % no_of_shaders == 3) {
        float intensity = varying_intensity.getX() + varying_intensity.getY() + varying_intensity.getZ();
        intensity /= 3.0;
        intensity = addAmbientFactor(intensity, ambient_factor);
        colour.multiply(intensity);
    }
    //Normal Information Via Colour
    else if(shader_type % no_of_shaders == 4) {
        colour.fill(255); //not the base_colour
        Vector4f sum = normals[0].add(normals[1].add(normals[2]));
        sum = sum.normalized();
        colour.multiply(sum);
    }

    return true;
}

float saturate(float intensity) {
    intensity = min(intensity, 1.0f);
    intensity = max(intensity, 0.0f);
    return intensity;
}

float addAmbientFactor(float intensity, float ambient_factor) {
    if(intensity < ambient_factor) {
        intensity += 1;
        intensity *= ambient_factor;
    }
    return saturate(intensity);
}

float contrast(float intensity, float factor) {
    float f = (1.0157 * (factor + 1))/(1.0157 - factor);
    float result = (f * (intensity - 0.5)) + 0.5;
    return saturate(result);
}
