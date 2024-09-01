#version 330 core
layout ( triangles ) in;
layout ( triangle_strip ,max_vertices = 100) out;

out vec2 TexCoord;
flat out int hudLevel;
out vec3 baryCoord;
out vec3 normal;


in VS_OUT {
    vec2 TexCoord;
    int vertexIndex;
    vec4 pos3d;
} gs_in[];

uniform float time;
uniform float ratio;
uniform int showVertexIndicies;
uniform int showNormals;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


void rectangle(int index){
    float size = 0.02;
    float cellSize = 0.10;
    int id = gs_in[index].vertexIndex;
    vec4 vertexPos = gl_in[index].gl_Position;

    //setting the rectangle as hud
    hudLevel = 1;
    size*= vertexPos.w; //scale rectangle according to its distance 

    vec2 cellPos = vec2(float(id%10),float(id/10));

    gl_Position = vertexPos + vec4(-size, -size*ratio, 0.0, 0.0);
    TexCoord = cellSize*(vec2(0.,1.) + cellPos);
    EmitVertex();

    gl_Position = vertexPos + vec4( -size, size*ratio, 0.0, 0.0);
    TexCoord = cellSize*(vec2(0.,0.)+ cellPos);
    EmitVertex();
    
    gl_Position = vertexPos + vec4( size, size*ratio, 0.0, 0.0);
    TexCoord = cellSize*(vec2(1.,0.)+ cellPos);
    EmitVertex();

    EndPrimitive();

    gl_Position = vertexPos + vec4(-size, -size*ratio, 0.0, 0.0);
    TexCoord = cellSize*(vec2(0.,1.)+ cellPos);
    EmitVertex();

    gl_Position = vertexPos + vec4(size, -size*ratio, 0.0, 0.0);
    TexCoord = cellSize*(vec2(1.,1.)+ cellPos);
    EmitVertex();
    
    gl_Position = vertexPos + vec4( size, size*ratio, 0.0, 0.0);
    TexCoord = cellSize*(vec2(1.,0.)+ cellPos);
    EmitVertex();

    EndPrimitive();
}
void triangleVertex(int index){
    hudLevel = 0;
    TexCoord = gs_in[index].TexCoord;
    baryCoord = vec3(index == 0, index == 1, index == 2);
    
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
}
void showNormal(vec3 n){
        
        hudLevel = 2;
        vec4 middle3d = (gs_in[0].pos3d + gs_in[1].pos3d + gs_in[2].pos3d)/3.;
        gl_Position = projMatrix*viewMatrix*middle3d ;
        EmitVertex();
        gl_Position = projMatrix*viewMatrix*middle3d + vec4(0.01);
        EmitVertex();
        gl_Position = projMatrix*viewMatrix*(middle3d + vec4(n.x,n.y,n.z,0.)/10.);
        EmitVertex();
        EndPrimitive();

}

void main() { 
    
        normal = normalize(cross(gs_in[1].pos3d.xyz - gs_in[0].pos3d.xyz,gs_in[2].pos3d.xyz - gs_in[0].pos3d.xyz));
        vec3 convexCenter = vec3(0.,0.,0.);
        if(dot(normal,convexCenter-gs_in[0].pos3d.xyz) > 0.){
            normal = - normal;
        }
        if(showNormals == 1){
            showNormal(normal);
        }

        //identity triangle
        triangleVertex(0);
		triangleVertex(1);
		triangleVertex(2);
		EndPrimitive();

       

        //rectangle at each corner ( PB : each rectangle is computed as many times as it has adjascent faces)
        if(showVertexIndicies == 1){ 
            rectangle(0);
            rectangle(1);
            rectangle(2);
        }
   
   

}  
