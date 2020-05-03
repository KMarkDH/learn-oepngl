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
    //不要再这里用mat3将uview或者uModel转为mat3,暂时还不明白为什么
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

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float contant;
    float linear;
    float quadraitc;
};

in vec2 TexCoor;
in vec3 Normal;
in vec3 VetCoor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 calcPointLight(Light light, vec3 normal, vec3 view)
{
    float distance = length(light.position - VetCoor);
    float attenuation = 1.0 / (light.contant + light.linear * distance + light.quadraitc * pow(distance, 2));


    vec3 ambient = attenuation * light.ambient * vec3(texture(material.ambientTex, TexCoor));

    vec3 lightDir = normalize(light.position - VetCoor);
    vec3 diffuse = attenuation * max(dot(lightDir, normal), 0.0f) * light.diffuse * vec3(texture(material.diffuseTex, TexCoor));

    vec3 viewDir = normalize(viewPos - VetCoor);
    vec3 reflectDir = normalize(reflect(-lightDir, normal));

    vec3 specular = attenuation * pow((max(dot(viewDir, reflectDir), 0.0)), material.shinnese) * light.specular * vec3(texture(material.specularTex, TexCoor));
    return ambient + diffuse + specular;
}

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightColor = calcPointLight(light, norm, viewPos);
    FragColor = vec4(lightColor.xyz, 1.0f);
}