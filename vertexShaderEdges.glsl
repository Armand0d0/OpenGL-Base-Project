#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
void main()
{
    

    vec4 pos3d = modelMatrix*vec4(pos.x, pos.y, pos.z, 1.0);

    gl_Position = projMatrix*viewMatrix*pos3d;
}