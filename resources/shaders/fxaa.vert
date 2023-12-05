#version 330 core

layout (location = 0) in vec2 inPosition; // Vertex position
layout (location = 1) in vec2 inTexCoords; // Texture coordinates

out vec2 texCoords; // Pass texture coordinates to fragment shader

void main() {
    texCoords = inTexCoords;
    gl_Position = vec4(inPosition, 0.0, 1.0);
}
