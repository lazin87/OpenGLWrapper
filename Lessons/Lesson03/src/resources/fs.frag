#version 330 core

in vec3 vsColor;
in vec2 vsTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;

void main() {
    vec2 vecCoords2 = vsTexCoord * 0.5F;
    fragColor = mix(texture(texture1, vecCoords2), texture(texture2, vec2(vsTexCoord.x, 1.0 - vsTexCoord.y)), 0.5);
    fragColor = fragColor * vec4(vsColor, 1.0F);
}
