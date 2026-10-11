#version 460 core

layout(std140, binding=0) uniform CameraUBO{
    mat4 m_proj;
    mat4 m_look;
    float nearClipPlane;
    float farClipPlane;
    float fov;
    float aspectRatio;
};

vec2 vertices[4] = {
    vec2(1, -1),
    vec2(1, 1),
    vec2(-1, -1),
    vec2(-1, 1)
};

layout(location=2)uniform vec3 position;
layout(location=3)uniform vec2 scale;
layout(location=4)uniform float angle;

layout(location=0)out vec2 uv;

void main(){
    float c = cos(angle);
    float s = sin(angle);

    /*mat4 m_scale = mat4(
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1
    );

    mat4 m_translate = mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        position.x, position.y, position.z, 1
    );

    mat4 m_rotation = mat4(
        c, s, 0, 0,
        -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

    mat4 m_model = m_translate * m_rotation * m_scale;*/

    mat4 m_model = mat4(
        c * scale.x, s * scale.x, 0, 0,
        -s * scale.y, c * scale.y, 0, 0,
        0, 0, 1, 0,
        position.x, position.y, position.z, 1
    );

    mat3 toUV = mat3(
        0.5, 0, 0,
        0, -0.5, 0,
        0.5, 0.5, 1
    );

    uv = (toUV * vec3(vertices[gl_VertexID], 1)).xy;

    gl_Position = m_proj * m_look * m_model * vec4(vertices[gl_VertexID], 0, 1);
}