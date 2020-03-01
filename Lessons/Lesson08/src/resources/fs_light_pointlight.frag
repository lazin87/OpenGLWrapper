#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float Kc;
    float Kl;
    float Kq;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main()
{
    vec3 texDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecColor = vec3(texture(material.specular, TexCoords));
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0F / (light.Kc + light.Kl * distance + light.Kq * distance * distance);

    // ambient
    vec3 ambient = attenuation * light.ambient * texDiffColor;

    // diffuse
    float diff = max(dot(lightDir, norm), 0.0F);
    vec3 diffuse = attenuation * light.diffuse * diff * texDiffColor;

    //specular
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(-FragPos);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0F), material.shininess);
    vec3 specular = attenuation * light.specular * spec * texSpecColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0F);
}
