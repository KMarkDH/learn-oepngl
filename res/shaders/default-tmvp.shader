@vertex
#version 330 core
layout (location = 0) in vec3 vCoor;
layout (location = 1) in vec2 tCoor;

out vec2 textureCoor;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
    gl_Position = perspective * view * model * transform * vec4(vCoor.xyz, 1.0f);
    textureCoor = tCoor;
}

@fragment
#version 330 core

in vec2 textureCoor;
uniform sampler2D texturePtr;

out vec4 dColor;

void main()
{
    dColor = texture(texturePtr, textureCoor);
}