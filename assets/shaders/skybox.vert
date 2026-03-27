#version 410 core

layout(location = 0) in vec3 position;

out vec3 textureDirection;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main(void) {
    textureDirection = position;
    vec4 clipPosition = projectionMatrix * viewMatrix * vec4(position, 1.0);
    gl_Position = clipPosition.xyww;
}
