#version 410 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTextureCoordinate;
layout (location = 2) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec2 textureCoordinate;
out vec3 fragmentNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main(void) {
    fragmentPosition = (modelMatrix * vec4(vertexPosition, 1.0f)).xyz;
    textureCoordinate = vertexTextureCoordinate;
    fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
    gl_Position = projectionMatrix * viewMatrix * vec4(fragmentPosition, 1.0f);
}
