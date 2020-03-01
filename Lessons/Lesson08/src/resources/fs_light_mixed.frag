#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float Kc;
    float Kl;
    float Kq;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;

#define POINT_LIGHTS_COUNT 4
uniform DirLight dirLight;
uniform PointLight pointLight[POINT_LIGHTS_COUNT];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < POINT_LIGHTS_COUNT; ++i) {
        result += CalcPointLight(pointLight[i], norm, viewDir, FragPos);
    }

    result += CalcSpotLight(spotLight, norm, viewDir, FragPos);

    FragColor = vec4(result,  1.0F);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 texDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecColor = vec3(texture(material.specular, TexCoords));
    vec3 lightDir = normalize(light.direction);

    vec3 ambient = light.ambient * texDiffColor;

    float diff = max(dot(-lightDir, normal), 0.0F);
    vec3 diffuse = light.diffuse * diff * texDiffColor;

    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0F), material.shininess);
    vec3 specular = light.specular * spec * texSpecColor;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 texDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecColor = vec3(texture(material.specular, TexCoords));
    vec3 lightDir = normalize(FragPos - light.position);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0F / (light.Kc + light.Kl * distance + light.Kq * distance * distance);

    vec3 ambient = attenuation * light.ambient * texDiffColor;

    float diff = max(dot(-lightDir, normal), 0.0F);
    vec3 diffuse = attenuation * light.diffuse * diff * texDiffColor;

    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0F), material.shininess);
    vec3 specular = attenuation * light.specular * spec * texSpecColor;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 texDiffColor = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecColor = vec3(texture(material.specular, TexCoords));
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(normalize(-light.direction), lightDir);
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0F, 1.0F);

    float diff = max(dot(lightDir, normal), 0.0F);
    vec3 diffuse = intensity * light.diffuse * diff * texDiffColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0F), material.shininess);
    vec3 specular = intensity * light.specular * spec * texSpecColor;

    return (diffuse + specular);
}
