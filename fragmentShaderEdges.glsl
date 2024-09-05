#version 330 core
uniform int showBackSideEdges;

void main()
{

    gl_FragColor = vec4(0.,1.,0.,1.);
    if(showBackSideEdges){
        gl_FragDepth = 0.;
    }
} 
