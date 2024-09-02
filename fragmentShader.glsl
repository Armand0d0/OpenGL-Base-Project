#version 330 core

in vec2 TexCoord;
flat in int hudLevel;
in vec3 baryCoord;
in vec3 normal;

uniform float time;
uniform sampler2D numbers;
uniform int showEdges;
uniform int showBackSideEdges;

void main(){

    gl_FragDepth = gl_FragCoord.z;

    if(hudLevel == 0){
        float wireDistance = 0.002/gl_FragCoord.w;
        bool showWire = (baryCoord.x < wireDistance || baryCoord.y < wireDistance || baryCoord.z < wireDistance);
        if(showWire && showEdges == 1){
            gl_FragColor =  vec4(0., .0, 0.0, 0.0);
            if(showBackSideEdges == 1){
                gl_FragDepth = 0.0001;
            }
        }else{

            vec3 lightPos = vec3(3.*cos(time),3.,3.*sin(time));
            vec3 lightDir = normalize(lightPos);
            vec4 lightcolor = vec4(1.0);
            float lightIntensity = 0.25;
            vec4 objectColor;
            if(true){
                objectColor = texture(numbers,TexCoord);
            }else{
                objectColor = vec4(1., .8, .8, 0.0);
            }
            gl_FragColor = 0.4*objectColor+ lightcolor*lightIntensity*dot(normal,lightDir);
        }

    }
    else if(hudLevel == 1){
        gl_FragColor =  texture(numbers,TexCoord);
        gl_FragDepth = 000;
    } else if(hudLevel == 2){
        gl_FragColor = vec4(1.,1.,0.,0.);
        if(showBackSideEdges == 1){
            gl_FragDepth = 0.01;
        }
    }
    
} 