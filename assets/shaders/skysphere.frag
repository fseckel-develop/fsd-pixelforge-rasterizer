#version 410 core

in vec2 textureCoordinate;
out vec4 fragmentColor;

uniform sampler2D skyTexture;


void main() {
    fragmentColor = texture(skyTexture, textureCoordinate);
}
