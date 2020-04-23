@vertex
#version 330 core
layout (location = 0) in vec3 vCoor;
layout (location = 1) in vec2 tCoor;
layout (location = 2) in vec3 iNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform vec3 lightPos;

out vec2 textureCoor;
out vec3 aNormal;
out vec3 aLightPos;
out vec3 vetPos;


void main()
{
    gl_Position = perspective * view * model * vec4(vCoor.xyz, 1.0f);
    textureCoor = tCoor;
    //计算法线矩阵,inverse开销巨大,应该在cpu中进行计算
    aNormal = mat3(transpose(inverse(view * model))) * iNormal;
    aLightPos = (view * vec4(lightPos.xyz, 1.0f)).xyz;

    vetPos = (view * model * vec4(vCoor.xyz, 1.0f)).xyz;
}

@fragment
#version 330 core
in vec2 textureCoor;
in vec3 aNormal;
in vec3 aLightPos;
in vec3 vetPos;

uniform sampler2D texturePtr;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;


out vec4 dColor;

void main()
{
    vec3 ambient = lightColor * ambientStrength;

    vec3 lightDir = normalize(aLightPos - vetPos);

    float diffuseFactor = max(dot(aNormal, lightDir), 0.0f);
    vec3 diffuse = lightColor * diffuseFactor * diffuseStrength;

    vec3 viewDir = normalize(-vetPos);
    vec3 reflectDir = reflect(-lightDir, aNormal);

    float specularFactor = pow(max(dot(reflectDir, viewDir), 0.0f), 32);
    vec3 specular = lightColor * specularStrength * specularFactor;

    vec3 result = ambient + diffuse + specular;
    dColor = texture(texturePtr, textureCoor) * vec4(result.xyz, 1.0f);
}