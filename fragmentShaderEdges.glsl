#version 330 core
uniform int showBackSideEdges;
uniform vec3 edgesColor;
void main()
{

    gl_FragColor = vec4(edgesColor.x,edgesColor.y,edgesColor.z,1.);

    if(showBackSideEdges == 1){
        gl_FragDepth = 0.;
    }
} 
