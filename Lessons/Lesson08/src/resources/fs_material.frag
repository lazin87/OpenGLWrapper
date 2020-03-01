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
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main()
{
    vec3 texColorDiff = vec3(texture(material.diffuse, TexCoords));
    vec3 texColorSpec = vec3(texture(material.specular, TexCoords));

    // ambient
    vec3 ambient = light.ambient * texColorDiff;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0F);
    vec3 diffuse = light.diffuse * diff * texColorDiff;

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0F), material.shininess);
    vec3 specular = light.specular * spec * texColorSpec;

    FragColor = vec4(ambient + diffuse + specular, 1.0F);
}
