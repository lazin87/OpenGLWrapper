#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0F);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5F;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0F), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 color = (ambient + diffuse + specular) * objectColor;

    fragColor = vec4(color, 1.0F);
}
