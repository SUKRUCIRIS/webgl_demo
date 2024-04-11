#version 300 es
precision mediump float;
out vec4 FragColor;
in vec2 texCoord;
flat in int texture_id;

uniform sampler2D textures[16];

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
}