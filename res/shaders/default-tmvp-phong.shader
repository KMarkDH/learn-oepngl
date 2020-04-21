@vertex
#version 330 core
layout (location = 0) in vec3 vCoor;
layout (location = 1) in vec2 tCoor;
layout (location = 2) in vec3 iNormal;

out vec3 vetPos;
out vec2 textureCoor;
out vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
    gl_Position = perspective * view * model * vec4(vCoor.xyz, 1.0f);
    textureCoor = tCoor;
    //计算法线矩阵,inverse开销巨大,应该在cpu中进行计算
    aNormal = mat3(transpose(inverse(model))) * iNormal;
    vetPos = (model * vec4(vCoor.xyz, 1.0f)).xyz;
}

@fragment
#version 330 core
in vec3 aNormal;
in vec2 textureCoor;
in vec3 vetPos;

uniform sampler2D texturePtr;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 dColor;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = lightColor * ambientStrength;

    vec3 lightDir = normalize(lightPos - vetPos);

    float diffuseFactor = max(dot(aNormal, lightDir), 0.0f);
    vec3 diffuse = lightColor * diffuseFactor;

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - vetPos);
    vec3 reflectDir = reflect(-lightDir, aNormal);

    float specularFactor = pow(max(dot(reflectDir, viewDir), 0.0f), 32);
    vec3 specular = lightColor * specularStrength * specularFactor;

    vec3 result = ambient + diffuse + specular;
    dColor = texture(texturePtr, textureCoor) * vec4(result.xyz, 1.0f);
}