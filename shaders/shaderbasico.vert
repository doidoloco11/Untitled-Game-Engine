#version 460 core

layout(std140, binding=0) uniform CameraUBO{
    mat4 m_proj;
    mat4 m_look;
    float nearClipPlane;
    float farClipPlane;
    float fov;
    float aspectRatio;
};

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_uv;

layout(location=0) uniform mat4 m_model;

layout(location=0) out vec3 normal;
layout(location=1) out vec2 uv;

void main(){
    normal = normalize(m_model * vec4(in_normal, 0)).xyz;
    uv = in_uv;
    gl_Position = m_proj * m_look * m_model * vec4(in_position, 1);
}