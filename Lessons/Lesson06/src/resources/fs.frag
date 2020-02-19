#version 330 core

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    fragColor = vec4(objectColor * lightColor, 1.0F);
}
