#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

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
    vec3 texDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecColor = vec3(texture(material.specular, TexCoords));
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // ambient
    vec3 ambient = light.ambient * texDiffColor;

    // check theta
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intenisty = clamp((theta - light.outerCutOff) / epsilon, 0.0F, 1.0F);

    // diffuse
    float diff = max(dot(lightDir, norm), 0.0F);
    vec3 diffuse = intenisty * light.diffuse * diff * texDiffColor;

    //specular
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(-FragPos);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0F), material.shininess);
    vec3 specular = intenisty * light.specular * spec * texSpecColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0F);
}
