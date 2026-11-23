#version 460 core

layout(location=0) out vec4 fragColor;

layout(location=3) uniform vec4 color;
layout(location=4) uniform sampler2D mainTexture;

layout(location=0) in vec2 uv;

void main(){
    fragColor = texture(mainTexture, uv) * color;
}