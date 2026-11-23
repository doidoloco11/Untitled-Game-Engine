#version 460 core

layout(location=0) out vec4 fragColor;

layout(location=0) in vec3 normal;
layout(location=1) in vec2 uv;

void main(){
    fragColor = vec4(1) * (0.2 + clamp(dot(normal, normalize(vec3(-1, 1, -4))), 0, 1) * 0.8);
}