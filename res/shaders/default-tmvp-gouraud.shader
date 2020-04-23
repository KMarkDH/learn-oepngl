@vertex
#version 330 core
layout (location = 0) in vec3 vCoor;
layout (location = 1) in vec2 tCoor;
layout (location = 2) in vec3 iNormal;

out vec2 textureCoor;
out vec4 result;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    gl_Position = perspective * view * model * vec4(vCoor.xyz, 1.0f);
    textureCoor = tCoor;
    //计算法线矩阵,inverse开销巨大,应该在cpu中进行计算
    vec3 aNormal = mat3(transpose(inverse(view * model))) * iNormal;
    vec3 vetPos = (view * model * vec4(vCoor.xyz, 1.0f)).xyz;
    vec3 aLightPos = (view * vec4(lightPos.xyz, 1.0f)).xyz;

    vec3 ambient = lightColor * ambientStrength;

    vec3 lightDir = normalize(aLightPos - vetPos);
    float diffuseFactor = max(dot(lightDir, aNormal), 0.0f);
    vec3 diffuse = diffuseStrength * diffuseFactor * lightColor;

    vec3 viewdir = normalize(-vetPos);
    vec3 reflectDir = reflect(-lightDir, aNormal);
    float specularFactor = pow(max(dot(viewdir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * specularFactor * lightColor;

    result = vec4((ambient + diffuse + specular).xyz, 1.0f);
}

@fragment
#version 330 core
in vec2 textureCoor;
uniform sampler2D texturePtr;

in vec4 result;

out vec4 dColor;

void main()
{
    dColor = texture(texturePtr, textureCoor) * result;
}