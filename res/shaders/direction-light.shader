@vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aCoor;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec2 texCoor;

void main()
{
    gl_Position = perspective * view * model * transform * vec4(aPos.xyz, 1.0f);
    texCoor = aCoor;
}

@fragment
#version 330 core
in vec2 texCoor;

uniform sampler2D texturePtr;

out vec4 aColor;

void main()
{
    aColor = texture(texturePtr, texCoor);
}