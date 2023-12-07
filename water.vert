#version 330 core

layout(location = 0) in vec3 aPos;   // Position attribute
layout(location = 1) in vec2 aTexCoord; // Texture coordinate attribute

out vec2 TexCoord; // Output texture coordinate to fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // Pass position to fragment shader
    TexCoord = aTexCoord; // Pass texture coordinate to fragment shader
}
