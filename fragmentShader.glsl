#version 330 core

in vec2 TexCoord;
flat in int hudLevel;
in vec3 baryCoord;
in vec3 baryFactor;
in vec3 normal;

uniform float time;
uniform sampler2D numbersTexture;
uniform sampler2D materialTexture;
uniform int showBackSideEdges;
uniform int showVertexIndices;
uniform int isEdge;
uniform vec4 edgesColor;
void main(){

    gl_FragDepth = gl_FragCoord.z;

    if(hudLevel == 0){
        if(isEdge == 1){
            gl_FragColor = edgesColor*0.7;
            if(showBackSideEdges == 1){
                gl_FragDepth = 0.01;
            }
        }else{
            vec3 lightPos = vec3(3.*cos(time),0.2,3.*sin(time));
            vec3 lightDir = normalize(lightPos);
            vec4 lightcolor = vec4(1.0);
            float lightIntensity = .4;
            vec4 objectColor;
            if(true){
                objectColor = texture(materialTexture,TexCoord);
            }else{
                objectColor = vec4(1., .8, .8, 0.0);
            }
            
            float angularFactor = max(dot(normal,lightDir),0.);
            gl_FragColor = 0.4*objectColor + lightcolor*lightIntensity*angularFactor;
        
        }
    }
    else if(hudLevel == 1){
        if(showVertexIndices == 1){
            gl_FragColor = texture(numbersTexture,TexCoord);
        }else{
            gl_FragColor = edgesColor*0.7;
        }
        if(showBackSideEdges == 1){
            gl_FragDepth = 0.00;
        }    
    } else if(hudLevel == 2){
        gl_FragColor = vec4(1.,1.,0.,0.);
        if(showBackSideEdges == 1){
        gl_FragDepth = 0.01;
    }
    }
    
} 