@vertex
#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uPerspective;

out vec2 TexCoor;
out vec3 Normal;
out vec3 VetCoor;

void main()
{
    gl_Position = uPerspective * uView * uModel * vec4(vertex.xyz, 1.0f);
    TexCoor = uv;
    
    Normal = normal;//mat3(transpose(inverse(uModel))) * normal;
    //不要再这里用mat3将uview或者uModel转为mat3,暂时还不明白为什么
    VetCoor = vec3((uModel * vec4(vertex.xyz, 1.0f)).xyz);
}





@fragment
#version 330 core

#define NR_POINT_LIGHTS 4

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    int shinnese;
};

struct DirectionLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;

    float contant;
    float linear;
    float quadraic;
};

struct FlashLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;
};

in vec2 TexCoor;
in vec3 Normal;
in vec3 VetCoor;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionLight directionLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform FlashLight flashLight;
uniform sampler2D texture_diffuse1;

vec3 calcDirectionLisht(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoor));

    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = max(dot(lightDir, normal), 0.0) * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoor));

    return ambient + diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = pow(max(dot(reflectDir, viewDir), 0.0), material.shinnese) * vec3(texture(material.texture_diffuse1, TexCoor));
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    float distance = length(light.position - VetCoor);
    float attenuation = 1.0f / (light.contant + light.linear * distance + light.quadraic * pow(distance, 2));

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoor));

    vec3 lightDir = normalize(light.position - VetCoor);
    vec3 diffuse = max(dot(lightDir, normal), 0.0) * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoor));

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = pow(max(dot(reflectDir, viewDir), 0.0), material.shinnese) * light.specular * vec3(texture(material.texture_diffuse1, TexCoor));
    return (ambient + diffuse + specular) * attenuation;
}

vec3 calcFlashLight(FlashLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - VetCoor);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoor));
    vec3 diffuse = max(dot(lightDir, normal), 0.0) * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoor));

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = pow(max(dot(reflectDir, viewDir), 0.0), material.shinnese) * light.specular * vec3(texture(material.texture_diffuse1, TexCoor));
    return (ambient + diffuse + specular) * intensity;
}

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir =  normalize(viewPos - VetCoor);
    vec3 result = calcDirectionLisht(directionLight, Normal, viewDir);
    // for (int i = 0; i < NR_POINT_LIGHTS; ++i)
    // {
    //     result += calcPointLight(pointLights[i], norm, viewDir);
    // }
    // result += calcFlashLight(flashLight, norm, viewDir);
    FragColor = vec4(result.xyz, 1.0f);
}