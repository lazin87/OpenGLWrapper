#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;
out vec2 TexCoords;

void main()
{
    vec4 posView = view * model * vec4(aPos, 1.0F);
    gl_Position = projection * posView;
    FragPos = vec3(posView);
    TexCoords = aTexCoords;

    LightPos = vec3(view * vec4(lightPos, 1.0F));
    Normal = normalMatrix * aNormal;
}
