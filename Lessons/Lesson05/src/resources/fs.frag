#version 330 core

in vec2 vsTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main() {
    fragColor = mix(texture(texture1, vsTexCoord), texture(texture2, vec2(vsTexCoord.x, 1.0 - vsTexCoord.y)), 0.5);
}
