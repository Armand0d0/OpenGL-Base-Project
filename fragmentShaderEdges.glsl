#version 330 core
uniform int showBackSideEdges;

void main()
{

    gl_FragColor = vec4(0.,1.,0.,1.);
    if(showBackSideEdges == 1){
        gl_FragDepth = 0.;
    }
} 
