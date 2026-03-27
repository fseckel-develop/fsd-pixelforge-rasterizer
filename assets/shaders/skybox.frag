#version 410 core

in vec3 textureDirection;

out vec4 fragmentColor;

uniform samplerCube skyBox;


void main(void) {
    fragmentColor = texture(skyBox, normalize(textureDirection));
}
