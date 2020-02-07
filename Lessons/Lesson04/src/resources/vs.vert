#version 330 core

layout(location=0) in vec3 pos;
layout(location=2) in vec2 texCoord;

out vec2 vsTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0F);
    vsTexCoord = texCoord;
}
