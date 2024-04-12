#version 300 es
precision mediump float;
out vec4 FragColor;
in vec2 texCoord;
in vec3 crntPos;
flat in int texture_id;

uniform sampler2D textures[16];
uniform float tempdivisor1;
uniform float tempdivisor2;
uniform float part1temp;
uniform float part2temp;
uniform float part3temp;
uniform vec3 degree20color;
uniform vec3 degree60color;
uniform float maketemp;

void main(){
	switch(texture_id) {
        case 0:
            FragColor = texture(textures[0], texCoord);
            break;
        case 1:
            FragColor = texture(textures[1], texCoord);
            break;
        case 2:
            FragColor = texture(textures[2], texCoord);
            break;
        case 3:
            FragColor = texture(textures[3], texCoord);
            break;
        case 4:
            FragColor = texture(textures[4], texCoord);
            break;
        case 5:
            FragColor = texture(textures[5], texCoord);
            break;
        case 6:
            FragColor = texture(textures[6], texCoord);
            break;
        case 7:
            FragColor = texture(textures[7], texCoord);
            break;
        case 8:
            FragColor = texture(textures[8], texCoord);
            break;
        case 9:
            FragColor = texture(textures[9], texCoord);
            break;
        case 10:
            FragColor = texture(textures[10], texCoord);
            break;
        case 11:
            FragColor = texture(textures[11], texCoord);
            break;
        case 12:
            FragColor = texture(textures[12], texCoord);
            break;
        case 13:
            FragColor = texture(textures[13], texCoord);
            break;
        case 14:
            FragColor = texture(textures[14], texCoord);
            break;
        case 15:
            FragColor = texture(textures[15], texCoord);
            break;
        default:
            FragColor = vec4(0.0); // Handle invalid texture_id
            break;
    }
    if(maketemp==1.0f){
    vec3 target;
    if(crntPos.x<tempdivisor1){
        target=(1.0f-(part1temp-20.0f)/40.0f)*degree20color+((part1temp-20.0f)/40.0f)*degree60color;
    }
    else if(crntPos.x<tempdivisor2){
        target=(1.0f-(part2temp-20.0f)/40.0f)*degree20color+((part2temp-20.0f)/40.0f)*degree60color;
    }
    else{
        target=(1.0f-(part3temp-20.0f)/40.0f)*degree20color+((part3temp-20.0f)/40.0f)*degree60color;
    }
    FragColor.xyz=FragColor.xyz*0.5f+target*0.5f;
    }
}