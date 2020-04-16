@vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoor;

out vec2 vertexColor;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
    gl_Position = perspective * view * model * transform * vec4(aPos.xyz, 1.0f);
    vertexColor = texCoor;
}

@fragment
#version 330 core

in vec2 vertexColor;
out vec4 aColor;

uniform sampler2D uTexture;

void main()
{
    aColor = texture(uTexture, vertexColor);
}