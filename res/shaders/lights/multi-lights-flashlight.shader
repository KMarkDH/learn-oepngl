@vertex
#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

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
    
    Normal = mat3(transpose(inverse(uModel))) * normal;
    VetCoor = vec3((uModel * vec4(vertex.xyz, 1.0f)).xyz);
}






@fragment
#version 330 core

struct Material
{
    sampler2D ambientTex;
    sampler2D diffuseTex;
    sampler2D specularTex;

    int shinnese;
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
uniform FlashLight flashLight;

vec3 calcFlashLight(FlashLight light, vec3 normal, vec3 view)
{
    vec3 ret = vec3(0.0f, 0.0f, 0.0f);
    
    vec3 ambient = light.ambient * vec3(texture(material.ambientTex, TexCoor));
    ret += ambient;

    vec3 lightDir = normalize(light.position - VetCoor);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
    vec3 diffuse = max(dot(lightDir, normal), 0.0f) * light.diffuse * vec3(texture(material.diffuseTex, TexCoor));

    vec3 viewDir = normalize(viewPos - VetCoor);
    vec3 reflectDir = normalize(reflect(-lightDir, normal));

    vec3 specular = pow((max(dot(viewDir, reflectDir), 0.0)), material.shinnese) * light.specular * vec3(texture(material.specularTex, TexCoor));
    ret += (diffuse + specular) * intensity;
    return ret;
}

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightColor = calcFlashLight(flashLight, norm, viewPos);

    FragColor = vec4(lightColor.xyz, 1.0f);
}