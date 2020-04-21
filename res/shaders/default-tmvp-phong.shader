@vertex
#version 330 core
layout (location = 0) in vec3 vCoor;
layout (location = 1) in vec2 tCoor;
layout (location = 2) in vec3 iNormal;

out vec2 textureCoor;
out vec3 aNormal;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
    gl_Position = perspective * view * model * transform * vec4(vCoor.xyz, 1.0f);
    textureCoor = tCoor;

    aNormal = iNormal;
}

@fragment
#version 330 core
in vec3 aNormal;
in vec2 textureCoor;

uniform sampler2D texturePtr;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 dColor;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = lightColor * ambientStrength;
    dColor = texture(texturePtr, textureCoor) * vec4(ambient.xyz, 1.0f);
}