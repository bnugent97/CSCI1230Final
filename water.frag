#version 330 core

out vec4 FragColor; // Output color of the pixel
in vec2 TexCoord; // Input texture coordinate from vertex shader

uniform float time; // Uniform variable for time
uniform sampler2D texture1; // Texture sampler

void main()
{
    // Create a simple wave effect using the sine function
    float wave = sin(TexCoord.x * 10.0 + time) * 0.05;

    // Perturb the texture coordinate with the wave effect
    vec2 perturbTex = TexCoord + vec2(wave, wave);

    // Sample the texture with the perturbed coordinates
    FragColor = texture(texture1, perturbTex);
}
