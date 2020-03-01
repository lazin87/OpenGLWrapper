#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main()
{
    vec3 diffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 specColor = vec3(texture(material.specular, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.direction);

    // ambient
    vec3 ambient = light.ambient * diffColor;

    // diffuse
    float diff = max(dot(norm, -lightDir), 0.0F);
    vec3 diffuse = light.diffuse * diff * diffColor;

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectLightDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectLightDir), 0.0F), material.shininess);
    vec3 specular = light.specular * spec * specColor;

    FragColor = vec4(ambient + diffuse + specular,  1.0F);
}
