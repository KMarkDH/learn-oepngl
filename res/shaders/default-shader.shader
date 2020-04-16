@vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 texCoor;
layout (location = 3) in vec2 texCoor1;
out vec4 vertexColor;
out vec2 TexCoor;
out vec2 TexCoor1;

uniform float aOffset;
uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
    gl_Position = perspective * view * model * transform * vec4(aPos.xyz, 1.0f);
    vertexColor = gl_Position;
    TexCoor = texCoor;
    TexCoor1 = texCoor1;
}

@fragment
#version 330 core

in vec4 vertexColor;
in vec2 TexCoor;
in vec2 TexCoor1;

uniform vec4 ourColor;
out vec4 aColor;

uniform sampler2D ourTexture;
uniform sampler2D myTexture;

uniform float filr;

void main()
{
    aColor = mix(texture(ourTexture, TexCoor), texture(myTexture, TexCoor1), filr);
}