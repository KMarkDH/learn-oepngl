@vertex
#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uPerspective;
uniform vec3 uLightPos;

out vec2 TexCoor;
out vec3 Normal;
out vec3 LightPos;
out vec3 VetCoor;

void main()
{
    gl_Position = uPerspective * uView * uModel * vec4(vertex.xyz, 1.0f);
    TexCoor = uv;
    Normal = mat3(transpose(inverse(uView * uModel))) * normal;

    //不要再这里用mat3将uview或者uModel转为mat3,暂时还不明白为什么
    LightPos = uLightPos;
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

struct DirectionLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 lightDirection;
};

in vec2 TexCoor;
in vec3 Normal;
in vec3 VetCoor;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionLight directionLight;

vec4 calcDirLight(DirectionLight dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.lightDirection);
    vec3 reflectDir = reflect(-lightDir, Normal);

    vec4 ambient = vec4(dirLight.ambient.xyz, 1.0f) * texture(material.ambientTex, TexCoor);
    vec4 diffuse = max(dot(lightDir, Normal), 0) * vec4(dirLight.diffuse.xyz, 1.0f) * texture(material.diffuseTex, TexCoor);
    vec4 specular = pow(max(dot(reflectDir, viewDir), 0), material.shinnese) * vec4(dirLight.specular.xyz, 1.0f) * texture(material.specularTex, TexCoor);

    return ambient + diffuse + specular;
}

out vec4 FragColor;

void main()
{

    vec4 directionLight = calcDirLight(directionLight, Normal, normalize(viewPos - Normal));
    FragColor = directionLight;
}