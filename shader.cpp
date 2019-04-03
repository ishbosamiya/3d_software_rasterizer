#include "shader.h"

Vector4f Shader::light_dir(cos(0), cos(0), cos(0));
int Shader::shader_type = 5;
int Shader::no_of_shaders = 8;
float Shader::ambient_factor = 0.05;
Colour Shader::base_colour;
Cubemap Shader::cubemap("cube_map");

Vertex Shader::vertexShader(Vertex vertex, int nth_vertex, int width, int height) {
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(width/2.0, height/2.0);
    Vertex result = vertex.transform(screen_space_transform).perspectiveDivide();

    if(shader_type > -1) {
        float lighting = result.getNormalSmooth().normalized().dot(light_dir.normalized());
        if(nth_vertex == 0) {
            positions[0] = vertex.getPosition();
            //positions[0] = result.getPosition();
            normals[0] = vertex.getNormalSmooth();
            varying_intensity.setX(lighting);
        }
        else if(nth_vertex == 1) {
            positions[1] = vertex.getPosition();
            //positions[1] = result.getPosition();
            normals[1] = vertex.getNormalSmooth();
            varying_intensity.setY(lighting);
        }
        else if(nth_vertex == 2) {
            positions[2] = vertex.getPosition();
            //positions[2] = result.getPosition();
            normals[2] = vertex.getNormalSmooth();
            varying_intensity.setZ(lighting);
        }
    }

    return result;
}

bool Shader::fragmentShader(Vector4f screen_coords, Vector4f barycentric_coords, Colour &colour) {
    //Unlit
    if(shader_type == -1) {
        return true;
    }

    //Texture + Lighting Smooth
    if(shader_type % no_of_shaders == 0) {
        float intensity = varying_intensity.dot(barycentric_coords);
        intensity = addAmbientFactor(intensity, ambient_factor);
        colour.multiply(intensity);
    }
    //Lighting Smooth Normals
    else if(shader_type % no_of_shaders == 1) {
        colour = base_colour;
        float intensity = varying_intensity.dot(barycentric_coords);

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
        Vector4f sum = interpolation(normals[0], normals[1], normals[2], barycentric_coords).normalized();
        colour.multiply(sum);
    }
    //Cube Map
    else if(shader_type % no_of_shaders == 5) {
        Vector4f gl_position = interpolation(positions[0], positions[1], positions[2], barycentric_coords);
//        cout << "gl_position: " << gl_position.getX() << " " << gl_position.getY() << " " << gl_position.getZ() << endl;
//        cout << "screen_coords: " << screen_coords.getX() << " " << screen_coords.getY() << " " << screen_coords.getZ() << endl;
        extern Vector4f camera_position;
        Vector4f gl_normal = interpolation(normals[0], normals[1], normals[2], barycentric_coords);
//        Vector4f result = reflect(screen_coords.sub(gl_position),
//                                  gl_normal);
        Vector4f result = reflect(camera_position.sub(gl_position),
                                  gl_normal);

        //colour = cubemap.textureCube(result);
        colour = cubemap.textureCube(gl_position);
    }
    //Vertex Colour
    else if(shader_type % no_of_shaders == 6) {
        colour.fill(255);
        colour.multiply(barycentric_coords);
    }
    //reflection
    else if(shader_type % no_of_shaders == 7) {
        Vector4f gl_position = interpolation(positions[0], positions[1], positions[2], barycentric_coords);
        extern Vector4f camera_position;
        Vector4f gl_normal = interpolation(normals[0], normals[1], normals[2], barycentric_coords);
        Vector4f result = reflect(camera_position.sub(gl_position),
                                  gl_normal);

        colour = cubemap.textureCube(result);
    }

    return true;
}

Vector4f Shader::interpolation(Vector4f v1, Vector4f v2, Vector4f v3, Vector4f barycentric) {
    Vector4f result;
    v1 = v1.mul(barycentric.getX());
    v2 = v2.mul(barycentric.getY());
    v3 = v3.mul(barycentric.getZ());
    result = v1.add(v2.add(v3));
    return result;
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

Vector4f reflect(Vector4f incident, Vector4f normal) {
    Vector4f r = normal.mul(2.0 * incident.dot(normal));
    return r.add(incident).normalized();
}
