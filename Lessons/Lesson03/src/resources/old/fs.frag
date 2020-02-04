#version 330 core

in vec3 vsColor;
in vec2 vsTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main() {
    vec2 vecCoords2 = vsTexCoord * 0.5F;
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);//mix(texture(texture1, vecCoords2), texture(texture2, vec2(vsTexCoord.x, 1.0 - vsTexCoord.y)), 0.5);
}
