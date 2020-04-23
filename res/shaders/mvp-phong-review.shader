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
    LightPos = vec3((uView * vec4(uLightPos.xyz, 1.0f)).xyz);
    VetCoor = vec3((uView * uModel * vec4(vertex.xyz, 1.0f)).xyz);
}


@fragment
#version 330 core
in vec2 TexCoor;
in vec3 Normal;
in vec3 LightPos;
in vec3 VetCoor;

uniform sampler2D uTexture0;
uniform vec3 uLightColor;
uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;

out vec4 FragColor;

void main()
{
    vec3 lightDir = normalize(LightPos - VetCoor);
    vec3 viewDir = normalize(-VetCoor);
    vec3 reflectDir = reflect(-lightDir, Normal);
    
    vec3 ambient = uAmbientStrength * uLightColor;

    float diffuseFct = max(dot(lightDir, Normal), 0.0f);
    vec3 diffuse = diffuseFct * uDiffuseStrength * uLightColor;

    float specularFct = pow(max(dot(reflectDir, viewDir), 0.0f), 128);
    vec3 specular = specularFct * uSpecularStrength * uLightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = texture(uTexture0, TexCoor) * vec4(result.xyz, 1.0f);
}