#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    vec4 posWorld = model * vec4(aPos, 1.0F);
    gl_Position = projection * view * posWorld;
    FragPos = vec3(posWorld);

    Normal = aNormal;
}
