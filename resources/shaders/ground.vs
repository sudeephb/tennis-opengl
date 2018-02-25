#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 groundmodel;
uniform mat4 groundview;
uniform mat4 groundprojection;

void main()
{
    gl_Position =  groundprojection * groundview * groundmodel * vec4(position, 1.0f);
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
