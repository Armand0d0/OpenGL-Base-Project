#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 aTexCoord;

out VS_OUT {
    vec2 TexCoord;
    int vertexIndex;
    vec4 pos3d;
} vs_out;

uniform float time;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
void main()
{
    
    vs_out.TexCoord = aTexCoord;
    vs_out.vertexIndex = gl_VertexID;
    vec4 pos3d = vec4(pos.x, pos.y, pos.z, 1.0);
    vs_out.pos3d = pos3d;

    gl_Position = projMatrix*viewMatrix*pos3d;
}